#pragma once

#include <complex>
#include <string>

using byte = unsigned char;
using cFloat = std::complex<float>;
using LogicalTime = unsigned long long;

#include <cstdint>
#include "Constants.hpp"
// #include "../../include/Types.hpp"

/*********************************************//**
 * @brief Compute the CRC16 checksum for the input
 * @param w_data Data to compute checksum for
 * @param w_dataLength Length of the input data
 * @result The CRC16 of the input data
 *
 * Compute the CRC16 checksum for the input.
 ************************************************/
uint32_t computeCRC16(const byte * w_data, unsigned int w_dataLength);

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
uint32_t checkCRC16(const byte * w_data, unsigned int w_dataLength);
