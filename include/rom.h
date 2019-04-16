#ifndef NES_ROM_H__
#define NES_ROM_H__

#include <6502.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NES_HEADER_SIZE    0x0010
#define NES_TRAINER_SIZE   0x0200
#define NES_PROM_BANK_SIZE 0x4000
#define NES_CROM_BANK_SIZE 0x2000

typedef struct NES_HEADER
{
	uint8_t m_nes[4];                 // Always 'NES' and a MSDOS EOF
	uint8_t m_PROMBankCount;          // 16,384 Bytes * Program   Rom 16k = 0x4000
	uint8_t m_CROMBankCount;         //   8,192 Bytes * Character Rom  8k = 0x2000

	uint8_t m_mirroring         : 1; // Mirroring: 0: horizontal (vertical arrangement) (CIRAM A10 = PPU A11)
								     //            1: vertical (horizontal arrangement) (CIRAM A10 = PPU A10)

	uint8_t m_batteryBacked     : 1; // 1: Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory
	uint8_t m_trainer           : 1; // 1: 512-byte trainer at $7000-$71FF (stored before PRG data)
	uint8_t m_fourScreenVRAM    : 1; // 1: Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM
	uint8_t m_lowerMapperNybble : 4; //

	uint8_t m_VSUnisystem       : 1; // VS Unisystem
	uint8_t m_reserved0         : 3; // Reserved, must be zeroes!
	uint8_t m_upperMapperNybble : 4; //

	uint8_t m_pramSize;              //  8,192 Bytes * Program Ram Size - Assume 1 8KB bank when zero
	uint8_t m_tvSystem          : 1; // TV system (0: NTSC; 1: PAL)
	uint8_t m_reserved1         : 7;

	uint8_t m_flag10;
	uint8_t m_zero[5];
}
NES_HEADER __attribute__ ((aligned(1)));

typedef struct NES_ROM
{
	uint8_t (*Read) ( struct MOS6502* em, uint16_t addr );
	void    (*Write)( struct MOS6502* em, uint16_t addr, uint8_t v );

	struct NES_HEADER*   m_header;
	struct NES_MAP*      m_map;

	uint8_t*             m_trainer;   // If exists contains hints info
	uint8_t**            m_PROM;      // Program ROM
	uint8_t**            m_CROM;      // Character ROM
}
NES_ROM;

/* */
NES_ROM* NesEx_CreateROM( char* fileDir );

#ifdef __cplusplus
}
#endif

#endif /* NES_ROM_H__ */