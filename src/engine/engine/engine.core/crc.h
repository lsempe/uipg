
#pragma once

#include <memory.h>

// CRC - Cyclic Redundancy Checksum Generator
//
// Returns the 16-bit or 32-bit CRC for a given block of 8-bit data
//
// 16-bit CRC uses the polynomial used by CRC-16 and XMODEM
// 32-bit CRC uses the polynomial used by PkZip, WinZip and Ethernet (reflected)
//
// Example use:
//
//		unsigned char data[] = "jfksDSFdjdjskbkds";
// 		unsigned short crc16 = CRC.GetCRC16((data), sizeof(data));
//		unsigned int crc32 = CRC.GetCRC32((data), sizeof(data));
//
//		float data[] = { 1.0, 44.0, 53.34, 45.3, 2.3 };
//		crc16 = CRC.GetCRC16(reinterpret_cast<unsigned char*> (data16bit), sizeof(data));
//		crc32 = CRC.GetCRC32(reinterpret_cast<unsigned char*> (data16bit), sizeof(data));
//

namespace sys
{

class CRC
{
protected:

	enum {
		POLYNOMIAL_16BIT = 0x1021,			// Used by CRC-16 and XMODEM
		CRC_INIT_16BIT   = 0x0000,			
		POLYNOMIAL_32BIT = 0x04c11db7,		// Used by CRC-32 in PkZip, WinZip and Ethernet
		CRC_INIT_32BIT   = 0xFFFFFFFF
	};

	unsigned short crc16_table[256];				// 16-Bit Look up table
	unsigned int crc32_table[256];				// 32-Bit Look up table

	bool   Initialized16;
	bool   Initialized32;

	unsigned int Reflect(unsigned int ref, char ch);	// Reflects CRC bits in the look up table

public:

	CRC() : Initialized16(false),
		Initialized32(false)
	{
		memset(crc16_table, 0, sizeof(unsigned short)*256);
		memset(crc32_table, 0, sizeof(unsigned int)*256);
	}

	~CRC() {}	

	static CRC& Get() 
	{
		static CRC crc;
		return crc;
	}

	unsigned short GetCRC16(const unsigned char* data, unsigned int length);
	unsigned int GetCRC32(const unsigned char* data, unsigned int length);

};

} // core