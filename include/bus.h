#ifndef NES_BUS_H__
#define NES_BUS_H__

#include <6502.h>
#include <stdint.h>
#include <stdbool.h>

#include "rom.h"
#include "ppu.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct NES_BUS
{
	/* Perform external operations based on CPU execution. */
	void    (*Tick ) ( struct MOS6502* em );

	/* Read Write to the CPU memory space. */
	uint8_t (*Read ) ( struct MOS6502* em, uint16_t addr );
	void    (*Write) ( struct MOS6502* em, uint16_t addr, uint8_t v );

	/* Counts the amount of ticks that have occurred. */
	const uint64_t m_ticks;

	/* NES Subsystem's */
	uint8_t  m_ram[ 0x800 ];
	NES_ROM* m_rom;
	NES_PPU* m_ppu;
	struct NES_APU* m_apu;
}
NES_BUS;

/*=================================================================
	CreateBus() - Will create a bus object for the NES.
===================================================================
	 fp -> Pointer to the object used by the 
	===============================================================
*/
NES_BUS* NesEx_CreateBUS( struct NES_ROM* rom, struct NES_PPU* ppu, struct NES_APU* apu );

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*NES_BUS_H_*/