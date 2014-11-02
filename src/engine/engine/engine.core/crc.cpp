#include "crc.h"
//#include "assert.h"

using namespace sys;

// 16 Bit CRC as used by CRC16 and XMODEM
unsigned short CRC::GetCRC16(const unsigned char* data, unsigned int length)
{
	// Create the look up table if it doesn't exist
	if (!Initialized16)
	{
		for(unsigned short i=0; i < 256; i++ )
		{
			unsigned short j = i << 8;
			for(unsigned short k=8; k--; )
			{
				j = j & 0x8000 ? (j << 1) ^ POLYNOMIAL_16BIT : j << 1;
			}
			crc16_table[ i ] = (unsigned short)j;
		}
		Initialized16 = true;
	}

	// Get the CRC
	unsigned short crc = CRC_INIT_16BIT;
	unsigned char* buffer = (unsigned char*)data;

	while (--length)
	{
		crc = (crc << 8) ^ crc16_table[((crc >> 8) & 0x00FF) ^ *buffer++];
	}

	return crc;
}

// Used only by 32 Bit CRC
unsigned int CRC::Reflect(unsigned int ref, char ch)
{
	unsigned int value = 0;

	// Swap bit 0 for bit 7, bit 1 for bit 6, etc.
	for (int i=1; i < (ch + 1); ++i)
	{
		if (ref & 1)
			value |= 1 << (ch - i);

		ref >>= 1;
	}
	return value;
}

// Used by CRC-32 in PkZip, WinZip and Ethernet (reflected)
unsigned int CRC::GetCRC32(const unsigned char* data, unsigned int length)
{
	if (!Initialized32)
	{
		for (int i=0; i < 0xFF; ++i)
		{
			crc32_table[i] = Reflect(i, 8) << 24;

			for (int j=0; j < 8; ++j)
			{
				crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? POLYNOMIAL_32BIT : 0);
			}

			crc32_table[i] = Reflect(crc32_table[i], 32);
		}
		Initialized32 = true;
	}

	// Get the CRC
	unsigned int crc = 0xffffffff;		
	unsigned char* buffer = (unsigned char*)data;

	while (--length)
	{
		crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];
	}

	return crc ^ CRC_INIT_32BIT;

}
