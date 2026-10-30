#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <optional>
#include <algorithm>


typedef unsigned char Byte;

constexpr size_t PKT_HEADER_SIZE  = 6;
constexpr size_t PKT_PAYLOAD_SIZE = 100;
constexpr size_t PKT_CRC_SIZE     = 2;
constexpr size_t PKT_TOTAL_BYTES  = 108;

const uint32_t CRC_TABLE_16[256] =
{
        0x0000,0x2110,0x4220,0x6330,0x8440,0xa550,0xc660,0xe770,
        0x0881,0x2991,0x4aa1,0x6bb1,0x8cc1,0xadd1,0xcee1,0xeff1,
        0x3112,0x1002,0x7332,0x5222,0xb552,0x9442,0xf772,0xd662,
        0x3993,0x1883,0x7bb3,0x5aa3,0xbdd3,0x9cc3,0xfff3,0xdee3,
        0x6224,0x4334,0x2004,0x0114,0xe664,0xc774,0xa444,0x8554,
        0x6aa5,0x4bb5,0x2885,0x0995,0xeee5,0xcff5,0xacc5,0x8dd5,
        0x5336,0x7226,0x1116,0x3006,0xd776,0xf666,0x9556,0xb446,
        0x5bb7,0x7aa7,0x1997,0x3887,0xdff7,0xfee7,0x9dd7,0xbcc7,
        0xc448,0xe558,0x8668,0xa778,0x4008,0x6118,0x0228,0x2338,
        0xccc9,0xedd9,0x8ee9,0xaff9,0x4889,0x6999,0xaa9,0x2bb9,
        0xf55a,0xd44a,0xb77a,0x966a,0x711a,0x500a,0x333a,0x122a,
        0xfddb,0xdccb,0xbffb,0x9eeb,0x799b,0x588b,0x3bbb,0x1aab,
        0xa66c,0x877c,0xe44c,0xc55c,0x222c,0x033c,0x600c,0x411c,
        0xaeed,0x8ffd,0xeccd,0xcddd,0x2aad,0x0bbd,0x688d,0x499d,
        0x977e,0xb66e,0xd55e,0xf44e,0x133e,0x322e,0x511e,0x700e,
        0x9fff,0xbeef,0xdddf,0xfccf,0x1bbf,0x3aaf,0x599f,0x788f,
        0x8891,0xa981,0xcab1,0xeba1,0x0cd1,0x2dc1,0x4ef1,0x6fe1,
        0x8010,0xa100,0xc230,0xe320,0x0450,0x2540,0x4670,0x6760,
        0xb983,0x9893,0xfba3,0xdab3,0x3dc3,0x1cd3,0x7fe3,0x5ef3,
        0xb102,0x9012,0xf322,0xd232,0x3542,0x1452,0x7762,0x5672,
        0xeab5,0xcba5,0xa895,0x8985,0x6ef5,0x4fe5,0x2cd5,0x0dc5,
        0xe234,0xc324,0xa014,0x8104,0x6674,0x4764,0x2454,0x0544,
        0xdba7,0xfab7,0x9987,0xb897,0x5fe7,0x7ef7,0x1dc7,0x3cd7,
        0xd326,0xf236,0x9106,0xb016,0x5766,0x7676,0x1546,0x3456,
        0x4cd9,0x6dc9,0x0ef9,0x2fe9,0xc899,0xe989,0x8ab9,0xaba9,
        0x4458,0x6548,0x0678,0x2768,0xc018,0xe108,0x8238,0xa328,
        0x7dcb,0x5cdb,0x3feb,0x1efb,0xf98b,0xd89b,0xbbab,0x9abb,
        0x754a,0x545a,0x376a,0x167a,0xf10a,0xd01a,0xb32a,0x923a,
        0x2efd,0x0fed,0x6cdd,0x4dcd,0xaabd,0x8bad,0xe89d,0xc98d,
        0x267c,0x076c,0x645c,0x454c,0xa23c,0x832c,0xe01c,0xc10c,
        0x1fef,0x3eff,0x5dcf,0x7cdf,0x9baf,0xbabf,0xd98f,0xf89f,
        0x176e,0x367e,0x554e,0x745e,0x932e,0xb23e,0xd10e,0xf01e
};


/*********************************************//**
* @brief Compute the CRC16 checksum for the input
* @param w_data Data to compute checksum for
* @param w_dataLength Length of the input data
* @result The CRC16 of the input data
*
* Compute the CRC16 checksum for the input.
************************************************/
uint32_t computeCRC16(const Byte * w_data, unsigned int w_dataLength)
{   
    uint32_t retVal = 0;

    int counter = 0;
    uint32_t c = 0;

    while(w_dataLength--)
    {
        c = (c >> 8) ^ CRC_TABLE_16[((c ^ w_data[counter++]) & 0xFF)];
    }

    retVal = c & 0x0000FFFF;

    return retVal;
}


/************************************************//**
* @brief Checks the CRC16 checksum for the input
* @param w_data Data to check checksum for
* @param w_dataLength Length of the input data
* @result Zero if matching; something else otherwise
*
* Compute the CRC16 checksum for the input. The
* checksum is to be placed in the two last bytes
* of the input array.
***************************************************/
uint32_t checkCRC16(const Byte * w_data, unsigned int w_dataLength)
{
  int counter = 0;
  uint32_t c = 0;

  unsigned int dl = w_dataLength;

  w_dataLength = w_dataLength - 2;

  while(w_dataLength--)
  {
      c = (c >> 8) ^ CRC_TABLE_16[((c ^ w_data[counter++]) & 0xFF)];
  }

  c = c & 0x0000FFFF;

  uint32_t checks = (((uint32_t)w_data[dl - 2]) << 8) | ((uint32_t)w_data[dl - 1]);
  
//   std::cout<<"check crc:"<<checks<<std::endl;

//  std::cout<<"check crc:"<<checks<<" tx crc:"<<c<<std::endl;

  c = c ^ (checks);
  
  return c;
}

// bits to bytes
std::vector<int> bytes_to_bits(std::vector<Byte> in)
{
   
   std::vector<int> out;

   
  int tmp;
  for(auto bt:in)
    {
    tmp = 0;
    for (int j = 0; j < 8; j++) {
      tmp = (((Byte)(bt << j)) >> 7);
      out.push_back(tmp);
    }
   }
    
    return out;

}
std::vector<Byte> bits_to_bytes(std::vector<int> in)
{
  int len = in.size()/8;
  std::vector<Byte> out(len);

  for (int i = 0; i < len; i++) {
      for (int j = 7; j >= 0; j--) {
         if (in[(i * 8) + j] != 0) {
                out[i] = out[i] + (1 << (7 - j));
          }
      }
  }
    return out;
}

// Int vector to vector of byte:
// Dat vector of 0 or 1
// Len byte amount 1 byte: {1,0,1,0,1,0,1,0}  (std::vector<int>.length / 8 = Len)
std::vector<Byte> to_byte_v(std::vector<int> dat) {

    int len = dat.size()/8;
    std::vector<Byte> res(len);

    for (int i = 0; i < len; i++) {
        for (int j = 7; j >= 0; j--) {
            if (dat[(i * 8) + j] != 0) {
                res[i] = res[i] + (1 << (7 - j));
            }
        }
    }

    return res;
}


// Byte to int vector:
// bt byte
std::vector<int> to_int(Byte bt) {
    std::vector<int> res(8);

    for (int j = 0; j < 8; j++) {
        res [j] = (((Byte)(bt << j)) >> 7);
    }

    return res;
}


// Int vector to byte:
// Dat vector of size 8 with values 0 or 1.
Byte to_byte(std::vector<int> dat) {
    Byte res;

    for (int j = 7; j >= 0; j--) {
        if (dat[j] != 0) {
            res = res + (1 << (7 - j));
        }
    }

    return res;
}

std::vector<Byte> attach_crc(std::vector<uint8_t> msg_in)
{
  // generate checksum
  std::vector<Byte> msg_out(msg_in);
  uint32_t checksum = computeCRC16( (uint8_t*) &msg_in[0], msg_in.size());
  // attach to the end
  
//  std::cout<<"initial crc:"<<checksum<<std::endl;
  msg_out.push_back((checksum>>8)& 0xFF);
  msg_out.push_back((checksum)& 0xFF);
  return msg_out;
}

int check_crc(std::vector<Byte> msg_in)
{
  int ret = 1;
  if(checkCRC16(&msg_in[0], msg_in.size()) == 0)
	  ret = 0;	
  return ret;	
}



// Turn MSG into binary:
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
std::vector<int> MSG_to_bin(std::string message) {
  int message_len = message.length();
  const char * msg = message.c_str();

  std::vector<Byte> payload_B(104);

  for (int i = 0; i < message_len && i < 104; i++) {
    payload_B[i] = (Byte)msg[i];
  }

  const Byte * pay_B = &payload_B[0];

  // generate crc for the packet 
  // crc is generated for bytes and given in bytes 
  uint32_t checksum = computeCRC16(pay_B, payload_B.size());

  // convert crc to bits 
  std::vector<int> checksum_bin(2 * 8);
  for(int i = 15; i >= 0; i--) {   // 2 bytes = 16 bits
    checksum_bin[i] = checksum & 1;
    checksum = checksum >> 1;
  }

  // convert bytes to bits 
  std::vector<int> pay_bin(104*8);
  for (int i = 0; i < 104; i++) {
    std::vector<int> new_Byte = to_int(pay_B[i]);
    for (int j = 0; j < 8; j++) {
      pay_bin[(i * 8) + j] = new_Byte[j];
    }
  }

  std::vector<int> txMsg(std::vector<int> (2 * 8));
  txMsg.insert(txMsg.end(), pay_bin.begin(), pay_bin.end());
  txMsg.insert(txMsg.end(), checksum_bin.begin(), checksum_bin.end());

  return txMsg;
}

std::string formatBinaryVector(const std::vector<int>& rx_bin_data) {
    std::ostringstream oss;
    for (size_t i = 0; i < rx_bin_data.size(); ++i) {
        oss << rx_bin_data[i] << " ";
    }
    return oss.str();
}



enum PacketType : uint8_t {
    PKT_DATA = 0x00,
    PKT_ACK  = 0x06,
    PKT_HELLO = 0x01,
    PKT_HELLO_ACK = 0x02
};

struct ParsedPacket {
    PacketType type = PKT_DATA;
    uint32_t seq_num = 0;
    std::string text = "";

    static std::optional<ParsedPacket> parse_packet(const std::vector<Byte>& msg) {
        if (msg.size() < 8 || check_crc(msg) != 0) {
            return std::nullopt;
        }

        ParsedPacket p;
        p.type = static_cast<PacketType>(msg[0]);
        p.seq_num = (msg[2] << 24) | (msg[3] << 16) | (msg[4] << 8) | msg[5];

        size_t len = std::min<size_t>(msg[1], msg.size() - 8);
        p.text.assign(msg.begin() + 6, msg.begin() + 6 + len);

        return p;
    }
};


inline std::vector<int> build_packet_bits(PacketType type, uint32_t seq_num, const std::string& text = "") {
    std::vector<uint8_t> bytes;
    bytes.reserve(PKT_TOTAL_BYTES);

    bytes.push_back(static_cast<uint8_t>(type));
    uint8_t text_len = static_cast<uint8_t>(std::min(text.size(), PKT_PAYLOAD_SIZE));
    bytes.push_back(text_len);

    bytes.push_back((seq_num >> 24) & 0xFF);
    bytes.push_back((seq_num >> 16) & 0xFF);
    bytes.push_back((seq_num >> 8)  & 0xFF);
    bytes.push_back( seq_num        & 0xFF);

    for (size_t i = 0; i < PKT_PAYLOAD_SIZE; ++i) {
        bytes.push_back(i < text_len ? static_cast<uint8_t>(text[i]) : 0x00);
    }

    std::vector<Byte> with_crc = attach_crc(bytes);
    return bytes_to_bits(with_crc);
}

inline std::vector<int> create_data_packet(uint32_t seq_num, const std::string& text) {
    return build_packet_bits(PKT_DATA, seq_num, text);
}

inline std::vector<int> create_ack_packet(uint32_t seq_num) {
    return build_packet_bits(PKT_ACK, seq_num, "");
}

inline std::vector<int> create_hello_packet() {
    return build_packet_bits(PKT_HELLO, 0, "HELLO");
}

inline std::vector<int> create_hello_ack_packet() {
    return build_packet_bits(PKT_HELLO_ACK, 0, "HELLO_ACK");
}