#include "CRC.hpp"

uint32_t computeCRC16(const byte * w_data, unsigned int w_dataLength)
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

uint32_t checkCRC16(const byte * w_data, unsigned int w_dataLength)
{
	int counter = 0;
	uint32_t c = 0;

	while(w_dataLength--)
	{
		c = (c >> 8) ^ CRC_TABLE_16[((c ^ w_data[counter++]) & 0xFF)];
	}

	c = c & 0x0000FFFF;

	return c;
}
