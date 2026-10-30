/*
  simple tx rx test 
 - pregenerates samples
 - repeats same transmission
 - records samples 
 */

/** C++

compilation:

cd build
cmake ..
make

to run if compiling again
cd.. to main folder
./rxtextmessage


g++ -std=c++17 demoUE2024.cpp Logger.cpp -lfmt -lfftw3f -lfftw3 -luhd -lboost_system -lboost_program_options -pthread -orxTextMessage

*/


/**************************************************************
	Headers
**************************************************************/
#include "Headers.hpp"
// #include "mainHeaders.hpp"

#include "./PHY_UE.hpp"
// #include "../PHY/PHY_UE.hpp"

// Data processing
#include "./Data_processing.hpp"

#include "Logger.hpp"


using namespace std;

static std::atomic<bool> STOP_SIGNAL_CALLED;
/**************************************************************
**************************************************************/



/**************************************************************
	Functions
**************************************************************/
void os_signal_handler(int s){
  printf("\n Caugth signal %d\n",s);

  STOP_SIGNAL_CALLED.store(true);
}



/**************************************************************
	Main
**************************************************************/
int UHD_SAFE_MAIN(int argc, char *argv[]) 
{
  int retVal = EXIT_SUCCESS;

  std::signal(SIGINT, os_signal_handler);

  Logger::initialize();

  int ue_tx_slot = 10;
  int txrx_chain_delay = 96;

  InterThreadQueueSDR7320<TXitem> txQueue; 
  InterThreadQueueSDR7320<RXitem> rxQueue; 


  PHY_UE phyUE(ue_tx_slot,txrx_chain_delay);
  phyUE.makeUEthread(txQueue,rxQueue);

  std::cout<<"UE main loops"<<std::endl;



  // random data generator
  int bits_in_one_rb = 12/2;   // how many bits are in on resource block
  int scaleSpectrum = 4;       // how many times the used OFDM symbol exceeds the min 128 
  int NRBDL = 6*scaleSpectrum; // how many resource blocs are in one OFDM symbol
  int fft_size = 128*scaleSpectrum; // final FFT size;
  
  RANDOM_DATA_FOR_PERFORMANCE_EST* testData = new RANDOM_DATA_FOR_PERFORMANCE_EST(fft_size,NRBDL);
  
  int nr_symbols = 6;          // how many OFDM symbols are in use
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
  // ACK        0x06     00000110
  // NAC        0x15     00010101
  //
  // NUL        0x00     00000000      Data packet

  //******************************************************************
  //******************************************************************

  // make msg header
  int header_size = 6;
  int crc_size = 2;
  int data_packet_size = (phy_packet_size/8) - header_size - crc_size;  

  int tx_acknack = 0; // 0 no ack nack 1 ack 2 nack
  int rx_acknack = 0; // 0 no ack nack 1 ack 2 nack

  //*********************************************************************
  // Additional code
  //*********************************************************************

  bool my_turn_to_transmit = false;
  bool ack_received = false;

  int state = 0;

  std::queue<bool> packet_correctness;

  size_t packet_no = 1;
  
  
  bool transmit_on = true;
  const int retransmit_numb = 500;
  int retransmit_count = retransmit_numb;
  int max_retransmissions = 500;
  int retransmissions = 0;
  bool tx_ready = false;

  std::string message;
  std::vector<int> txMsg;

  //*********************************************************************
  //*********************************************************************

  int attached = 0; // 0 not attached 1 attached 2 attach process going on 

  uint64_t old_rx_count = 0;
  uint64_t rx_count = 0;
  uint64_t diff_rx_count=0;
  uint64_t old_tx_count = 0;
  uint64_t tx_count = 0;
  int waitForTx=20;

bool handshake_done = false;
std::string attach_request = "HELLO";
std::string attach_accept = "HELLO_ACK";
auto last_send_time = std::chrono::steady_clock::now() - std::chrono::seconds(10);

while (!handshake_done && !STOP_SIGNAL_CALLED.load()) {

   auto now = std::chrono::steady_clock::now();
   if ( now - last_send_time >= std::chrono::seconds(2)) {
    // Send HELLO
    std::vector<uint8_t> msg_bytes = {0x01, (uint8_t)attach_request.size(), 0, 0, 0, 0};
    for (int i = 0; i < data_packet_size; ++i) {
        uint8_t sym = (i < attach_request.size()) ? attach_request[i] : 0;
        msg_bytes.push_back(sym);
    }
    auto msg_bytes_crc = attach_crc(msg_bytes);
    auto tx_bits = bytes_to_bits(msg_bytes_crc);
    Logger::getLogger()->info("sent message bits{}", formatBinaryVector(tx_bits));

    auto txItem = std::make_unique<TXitem>();
    txItem->insertData(std::move(tx_bits));
    txItem->tx_meta_data.num_tx_symbols = 6;
    txQueue.writeItem(std::move(txItem));

    std::cout << "Sent attach request: " << attach_request << std::endl;
    last_send_time = now;

  }

    // Wait for reply
    auto start = std::chrono::steady_clock::now();
    bool got_response = false;
    while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(500)) {
        auto tmp = std::move(rxQueue.readItem());
        if (tmp) {
            std::vector<int> rx_bin_data;
            for (int i = 0; i < phy_packet_size; i++)
                rx_bin_data.push_back((int)(tmp.get()->elem[i] < 0));
                Logger::getLogger()->info("received message bits{}", formatBinaryVector(rx_bin_data));


            auto rx_msg = bits_to_bytes(rx_bin_data);
            if (check_crc(rx_msg) == 0) {
                std::string recv_str(rx_msg.begin() + 6, rx_msg.begin() + 6 + rx_msg[1]);
                if (recv_str == attach_accept) {
                    std::cout << "Handshake successful. BS replied with: " << recv_str << std::endl;
                    handshake_done = true;
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


  
    // while stop condition becomes true
  while(true){
    if(STOP_SIGNAL_CALLED.load()) break;


    std::this_thread::sleep_for(std::chrono::nanoseconds(3000));

   // this synchronizes the tx and rx buffers
    if(diff_rx_count > 0)
    {
      waitForTx++;
      diff_rx_count--;
    }

    //********************************************************************
    // tx 
    //********************************************************************
      if(ack_received && !(waitForTx%100))
        {

          if (STOP_SIGNAL_CALLED.load()) break;;
          std::cout << "************** Rx end ********************" << std::endl << std::endl << std::endl << std::endl << std::endl;
          std::cout << "************** Tx ************************" << std::endl << std::endl;
          std::cout << "Input new message:" << std::endl << std::endl;

          char c_message[105];
          std::cin.get(c_message, 105);
          std::cin.clear();
          std::cin.ignore(100000, '\n');

          if (STOP_SIGNAL_CALLED.load()) break;;
          message = std::string(c_message);

          ack_received = false;
          my_turn_to_transmit = true;


      
          if (my_turn_to_transmit)
          {
              //================== Build the packet ==================
              std::vector<uint8_t> msg_bytes;
              msg_bytes.push_back(0x0F);                              // Header
              msg_bytes.push_back((uint8_t)message.size());          // Length
              msg_bytes.push_back((packet_no >> 24) & 0xFF);          // Seq num (4 bytes)
              msg_bytes.push_back((packet_no >> 16) & 0xFF);
              msg_bytes.push_back((packet_no >> 8) & 0xFF);
              msg_bytes.push_back(packet_no & 0xFF);

              // Append message content (pad with zeros if shorter than data_packet_size)
              for (int i = 0; i < data_packet_size; ++i)
              {
                  uint8_t sym = (i < message.size()) ? message[i] : 0;
                  msg_bytes.push_back(sym);
              }

              // Add CRC and convert to bits
              std::vector<uint8_t> msg_bytes_crc = attach_crc(msg_bytes);
              std::vector<int> tx_msg_bits = bytes_to_bits(msg_bytes_crc);

              //================== Send the packet ==================
              std::vector<int> txData(tx_msg_bits);
              auto txItem = std::make_unique<TXitem>();
              txItem->insertData(std::move(txData));
              txItem->tx_meta_data.num_tx_symbols = 6;  // adjust if needed
              txQueue.writeItem(std::move(txItem));

              std::cout << "Packet (tx packet no: " << packet_no << ") sent with message:" << std::endl << message << std::endl << std::endl;
              Logger::getLogger()->info("Packet sent (tx packet no: {}), message:\n\n{}\n", packet_no, message);

              // Reset state
              tx_ready = false;
              my_turn_to_transmit = false;
              packet_no++;
          }
    
          
        }
        /**************************************************************
        **************************************************************/



        //********************************************************************
        // rx 
        //********************************************************************
      while(true){
      
        auto tmp = std::move(rxQueue.readItem()); // reads the received data from PHY
        if(tmp != nullptr)                        // received a data packet
        {
          
          std::vector<int> rx_bin_data;
          for (int i = 0; i < phy_packet_size; i++) {
            rx_bin_data.push_back((int)(tmp.get()->elem[i]<0));
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
        auto crc = check_crc(rx_msg);

    
        if(crc == 0)
        {
            uint32_t tmp_pkt_nr = 0;
            tmp_pkt_nr = (((uint32_t)rx_msg[2])<<24)|(((uint32_t)rx_msg[3])<<16)|(((uint32_t)rx_msg[4])<<8)|(((uint32_t)rx_msg[5]));
            std::cout<<"pkt nr: "<<tmp_pkt_nr<<" bits:";
            for(int i1 = 0;i1<24;i1++)
                std::cout<<(int) rx_bin_data[i1]<<" ";
            std::cout<<std::endl;
            Logger::getLogger()->info("Received binary data: {}", formatBinaryVector(rx_bin_data));
            ack_received = true;

            size_t msg_len = rx_msg[1];
            if (rx_msg.size() < 6 + msg_len) {
            std::cerr << "Error: rx_msg size smaller than expected message length" << std::endl;

            }

            std::string message_text(rx_msg.begin() + 6, rx_msg.begin() + 6 + msg_len);
            std::cout <<"Received Message: \n\n "<< message_text << "\n" << std::endl;
            Logger::getLogger()->info("Received message text: \n\n{}\n", message_text);

        }

        }
       // (tmp != nullptr)  -> rx packet 
        else
        {
          break;
        }
        
        /**************************************************************
        **************************************************************/
        } // rx// rx// rx
    
 } // while


  // send stop to the phy thread
  phyUE.stopProcessing();
 
  std::this_thread::sleep_for(std::chrono::nanoseconds(100) ); // placeholder for processing

  std::cout<<"Exit the main function"<<std::endl;

  return retVal;
}
