#ifndef __NES_ROM_H__
#define __NES_ROM_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define NES_HEADER_SIZE    0x10
#define NES_TRAINER_SIZE   0x200
#define NES_ROM_BANK_SIZE  0x4000
#define NES_CROM_BANK_SIZE 0x2000

/* Defining shorthand signed/unsigned intigers of varying size */
typedef int8_t   s8; typedef uint8_t   u8;
typedef int16_t s16; typedef uint16_t u16;
typedef int32_t s32; typedef uint32_t u32;
typedef int64_t s64; typedef uint64_t u64;

typedef struct __NES_HEADER
{
	u8 m_nes[4];               // Always NES and a MSDOS EOF
	u8 m_programRomSize;       // 16,384 Bytes * Program   Rom 16k = 0x4000
	u8 m_characterRomSize;     //  8,192 Bytes * Character Rom     = 0x2000

	u8 m_mirroring         : 1; // Mirroring: 0: horizontal (vertical arrangement) (CIRAM A10 = PPU A11)
								//            1: vertical (horizontal arrangement) (CIRAM A10 = PPU A10)
	u8 m_batteryBacked     : 1; // 1: Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory
	u8 m_traniner          : 1; // 1: 512-byte trainer at $7000-$71FF (stored before PRG data)
	u8 m_fourScreenVRAM    : 1; // 1: Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM
	u8 m_lowerMapperNybble : 4; //

	u8 m_VSUnisystem       : 1; // VS Unisystem
	u8 m_reserved0         : 3; // Reserved, must be zeroes!
	u8 m_upperMapperNybble : 4; // 

	u8 m_programRamSize;        //  8,192 Bytes * Program Ram Size - Assume 1 8KB bank when zero
	u8 m_tvSystem  : 1;         // TV system (0: NTSC; 1: PAL)
	u8 m_reserved1 : 7;
	u8 m_flag10;
	u8 m_zero[5];
}
NES_HEADER;

typedef struct __NES_FILE 
{
	/* NES Data */
	struct __NES_HEADER* m_header;
	u8*                  m_traniner;   // If exists containes hints info
	u8**                 m_ROM;        // Pointer to application rom    
	u8**                 m_CROM;       // Pointer to character rom   
	/* Selected Banks  */
	u8                   m_mapperId;
	u8                   m_selectedBank[2];
	u8                   m_selectedCharacterROM;
}
NES_FILE;

/*=========================================================
	LoadNESROM()
==========================================================
	*Load and partiotion the NES executable from disk.
*/
NES_FILE* LoadROM( char* file );

#endif