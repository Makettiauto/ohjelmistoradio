/*
  simple tx rx test
 - pregenerates samples
 - repeats same transmission
 - records samples
 */

/** C++

compilation:

g++ -std=c++17 demoBS2024.cpp Logger.cpp -lfmt  -lfftw3f -lfftw3 -luhd -lboost_system -lboost_program_options -pthread -otxTextMessage

*/

/**************************************************************
  Headers
**************************************************************/
// #include "mainHeaders.hpp"
#include "Headers.hpp"

#include "./PHY_BS.hpp"

// Data processing
#include "./Data_processing.hpp"

using namespace std;

const auto ACK_TIMEOUT = std::chrono::milliseconds(500);
const int MAX_RETRIES = 5;

static std::atomic<bool> STOP_SIGNAL_CALLED;

void os_signal_handler(int s)
{
  printf("\n Caught signal %d\n", s);
  STOP_SIGNAL_CALLED.store(true);
}

/**************************************************************
  Main
**************************************************************/
int UHD_SAFE_MAIN(int argc, char *argv[])
{
  int retVal = EXIT_SUCCESS;

  std::signal(SIGINT, os_signal_handler);

  int ue_tx_slot = 10;
  int txrx_chain_delay = 95;

  // Queues for communicating between threads
  InterThreadQueueSDR7320<TXitem> txQueue; // TX queue from the main to PHY thread
  InterThreadQueueSDR7320<RXitem> rxQueue; // RX quewe from the PHY thread to main

  PHY_BS phyBS(ue_tx_slot, txrx_chain_delay); // Create BS instance
  phyBS.makeBSthread(txQueue, rxQueue);       // Start BS thread

  // random data generator
  int bits_in_one_rb = 12 / 2;        // how many bits are in on resource block
  int scaleSpectrum = 4;              // how many times the used OFDM symbol exceeds the min 128
  int NRBDL = 6 * scaleSpectrum;      // how many resource blocs are in one OFDM symbol
  int fft_size = 128 * scaleSpectrum; // final FFT size;

  RANDOM_DATA_FOR_PERFORMANCE_EST *testData = new RANDOM_DATA_FOR_PERFORMANCE_EST(fft_size, NRBDL);

  int nr_symbols = 6; // how many OFDM symbols are in use
  int phy_packet_size = NRBDL * bits_in_one_rb * nr_symbols;

  //******************************************************************
  // Data packet:
  //******************************************************************

  // 2 byte header, 104 byte payload and 2 byte crc. 1 packet = 108 bytes = 864 bits.
  //
  // Header symbols:
  //
  // symbol:    hex:     bin:          comment:
  //
  // ATTACH     0x09     00001001      used in initial attachment
  //
  // ACK        0x06     00000110
  // NAC        0x15     00010101
  //
  // NUL        0x00     00000000      Data packet

  //******************************************************************
  //******************************************************************
  int header_size = 6;
  int crc_size = 2;

  int data_packet_size = (phy_packet_size / 8) - header_size - crc_size;

  //*********************************************************************
  // Additional code
  //*********************************************************************

  bool my_turn_to_transmit = true;
  bool waiting_for_ack = false;
  std::vector<int> pending_tx_bits;
  auto last_tx_time = std::chrono::steady_clock::now();
  int retry_count = 0;

  std::queue<bool> packet_correctness;

  uint32_t packet_no = 1;

  bool transmit_on = true;
  const int retransmit_numb = 500;
  int retransmit_count = retransmit_numb;

  /*
     Example data packet
     Packet size in bytes: phy_packet_size/8 - header_size - crc_sise
  */
  std::string message = "Hello world!";
  if (argc > 1)
  {
    message = "";
    for (int i = 1; i < argc; i++)
    {
      message.append(argv[i]);
      message.append(" ");
    }
  }
  // convert message to bytes

  uint8_t sym;

  //******************************************************************
  //******************************************************************

  int state = 0;
  int rx_acknack = 0;
  int tx_acknack = 0;
  bool tx_ready = false;
  std::cout << "BS main loops" << std::endl;
  uint64_t old_rx_count = 0;
  uint64_t rx_count = 0;
  uint64_t diff_rx_count = 0;
  uint64_t old_tx_count = 0;
  uint64_t tx_count = 0;
  int waitForTx = 20;

  bool handshake_done = false;

  while (!handshake_done && !STOP_SIGNAL_CALLED.load())
  {
    auto tmp = std::move(rxQueue.readItem()); // reads the received data from PHY
    if (tmp != nullptr)                       // received a data packet
    {
      // prealocate phy_packet_size vector
      std::vector<int> rx_bin_data(phy_packet_size);
      for (int i = 0; i < phy_packet_size; i++)
      {
        rx_bin_data[i] = (int)(tmp.get()->elem[i] < 0);
      }

      auto rx_msg = bits_to_bytes(rx_bin_data);
      auto pkt = ParsedPacket::parse_packet(rx_msg);

      if (pkt && pkt->type == PKT_HELLO)
      {
        std::cout << "UE sent attach request. Sending HELLO_ACK." << std::endl;

        auto txItem = std::make_unique<TXitem>();
        txItem->insertData(create_hello_ack_packet());
        txItem->tx_meta_data.num_tx_symbols = 6;
        txQueue.writeItem(std::move(txItem));

        handshake_done = true;
      }
    }
    std::this_thread::sleep_for(std::chrono::nanoseconds(5000));
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  uint32_t last_rx_seq = 0;

  while (true)
  {
    if (STOP_SIGNAL_CALLED.load())
      break; // catch ctrl-C and end the code cleanly

    std::this_thread::sleep_for(std::chrono::nanoseconds(5000));

    // this synchronizes the tx and rx buffers
    if (diff_rx_count > 0)
    {
      waitForTx++;
      diff_rx_count--;
    }

    //********************************************************************
    // tx
    //********************************************************************
    if (my_turn_to_transmit && !waiting_for_ack && !(waitForTx % 100))
    {

      if (STOP_SIGNAL_CALLED.load())
        break;
      std::cout << "************** Rx end ********************" << std::endl
                << std::endl
                << std::endl
                << std::endl
                << std::endl;
      std::cout << "************** Tx ************************" << std::endl
                << std::endl;
      std::cout << "Input new message:" << std::endl
                << std::endl;

      char c_message[105];
      std::cin.get(c_message, 105);
      std::cin.clear();
      std::cin.ignore(100000, '\n');

      if (STOP_SIGNAL_CALLED.load())
        break;
      message = std::string(c_message);

      pending_tx_bits = create_data_packet(packet_no, message);

      //================== Send the packet ==================
      auto txItem = std::make_unique<TXitem>();
      txItem->insertData(pending_tx_bits);
      txItem->tx_meta_data.num_tx_symbols = 6; // adjust if needed
      txQueue.writeItem(std::move(txItem));

      std::cout << "Packet (tx packet no: " << packet_no << ") sent with message:" << std::endl
                << message << std::endl
                << std::endl;

      // Reset state
      waiting_for_ack = true;
      my_turn_to_transmit = false;
      last_tx_time = std::chrono::steady_clock::now();
      retry_count = 0;
    }

    // Restransmition logic
    if (waiting_for_ack)
    {
      auto now = std::chrono::steady_clock::now();
      if (now - last_tx_time > ACK_TIMEOUT)
      {
        if (retry_count < MAX_RETRIES)
        {
          retry_count++;
          std::cout << "[BS Timeout] Resending seq " << packet_no
                    << " (attempt " << retry_count << "/" << MAX_RETRIES << ")..." << std::endl;

          auto retryItem = std::make_unique<TXitem>();
          retryItem->insertData(std::vector<int>(pending_tx_bits));
          retryItem->tx_meta_data.num_tx_symbols = 6;
          txQueue.writeItem(std::move(retryItem));

          last_tx_time = now;
        }
        else
        {
          std::cout << "[BS Error] Max retries reached for seq " << packet_no << ". Giving up on packet." << std::endl;
          waiting_for_ack = false;
          packet_no++;
        }
      }
    }

    /**************************************************************
    **************************************************************/

    //********************************************************************
    // rx
    //********************************************************************
    while (true)
    {
      if (STOP_SIGNAL_CALLED.load())
        break; // catch ctrl-C and end the code cleanly

      auto tmp = std::move(rxQueue.readItem()); // reads the received data from PHY
      if (tmp != nullptr)                       // received a data packet
      {

        // prealocate phy_packet_size vector
        std::vector<int> rx_bin_data(phy_packet_size);
        for (int i = 0; i < phy_packet_size; i++)
        {
          rx_bin_data[i] = (int)(tmp.get()->elem[i] < 0);
        }

        // for syncing tx and rx
        // we compute the difference between the current rx_slot nr and previous one.
        // it should be about 20. We keep the update rate of waitForTx in the same rate
        old_rx_count = rx_count;
        rx_count = tmp.get()->rx_meta_data.slots;
        diff_rx_count = rx_count - old_rx_count;
        //      std::cout<<"rx:"<<tmp.get()->rx_meta_data.slots<<" "<<waitForTx<<std::endl;

        tmp.reset();

        // check CRC
        std::vector<Byte> rx_msg = bits_to_bytes(rx_bin_data);
        auto pkt = ParsedPacket::parse_packet(rx_msg);

        if (pkt)
        {
          if (pkt->type == PKT_ACK && waiting_for_ack && pkt->seq_num == packet_no)
          {
            std::cout << "ACK received from UE for seq: " << pkt->seq_num << std::endl;
            waiting_for_ack = false;
            packet_no++;
          }
          else if (pkt->type == PKT_DATA)
          {

            // Transmit an ACK back to the UE
            auto ackItem = std::make_unique<TXitem>();
            ackItem->insertData(create_ack_packet(pkt->seq_num));
            ackItem->tx_meta_data.num_tx_symbols = 6;
            txQueue.writeItem(std::move(ackItem));

            // print if not duplicate
            if (pkt->seq_num != last_rx_seq)
            {
              std::cout << "\n[Received msg (seq " << pkt->seq_num << ")]: " << pkt->text << "\n"
                        << std::endl;
              last_rx_seq = pkt->seq_num;

              // Hand over the turn so this terminal can reply
              my_turn_to_transmit = true;
            }
          }
        }
      } // (tmp != nullptr)  -> rx packet
      else
      {
        break;
      }

      /**************************************************************
      **************************************************************/
    } // rx
    /**************************************************************
    **************************************************************/

  } // while

  // send stop to the phy thread
  phyBS.stopProcessing();

  delete testData;

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  std::cout << "Exit the main function" << std::endl;

  return retVal;
}
