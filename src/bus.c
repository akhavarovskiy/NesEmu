#include <stdlib.h>
#include <stdio.h>
#include "..//include//bus.h"
#include "..//include//map.h"
#include "..//include//jpd.h"

#ifdef __cplusplus
extern "C" {
#endif

static uint8_t BUS_Read( MOS6502* em, uint16_t addr )
{	
	struct NES_BUS* b = (struct NES_BUS*)em->m_bus;

	switch (addr)
	{
		/* CPU RAM Address Space with mirroring */
		case 0x0000 ... 0x1FFF:
			return b->m_ram[ addr & 0x7FF ];
	
		/* PPU Register with mirroring */
		case 0x2000 ... 0x3FFF:
			return b->m_ppu->Read( em, addr );
			
		/* APU & IO Registers */
		case 0x4000 ... 0x4017:
			switch(addr & 0x1F)
			{
				case 0x14: 
					break;
				case 0x15:  
					//APU::Read();   
					break;
				case 0x16:
					return ReadState(0);
					break;
				case 0x17://if(!write) return IO::JoyRead(1); // write:passthru
					break;
				default: 
					break;
			}
			break;

		/* ROM Address Space */
		case 0x4018 ... 0xFFFF:
			return b->m_rom->Read( em, addr );
	}
	return 0x0000;
}

static void BUS_Write( MOS6502* em, uint16_t addr, uint8_t v )
{
	struct NES_BUS* b = (struct NES_BUS*)em->m_bus;

	switch (addr)
	{
		/* CPU RAM Address Space with mirroring */
		case 0x0000 ... 0x1FFF:
			b->m_ram[ addr & 0x7FF ] = v;
			break;

		/* PPU Register with mirroring */
		case 0x2000 ... 0x3FFF:
			b->m_ppu->Write( em, addr, v );
			break;

		/* APU & IO Registers */
		case 0x4000 ... 0x4017:
			switch(addr & 0x1F)
			{
				/* OAM DMA: Copy 256 bytes from RAM into PPU's sprite memory */
				case 0x14: 
					for( unsigned i = 0; i < 256; i++ ) {
						b->m_ppu->Write( em, 0x2004, BUS_Read( em, ((u16)v << 8) + i ) );
					}
					break;
				case 0x15: 
					break;
				case 0x16: 
					WriteStrobe( v & 1 );
					break;
				case 0x17:
					break;
				default: 
					//APU::Write(addr&0x1F, val);
					break;
			}
			break;

		/* ROM Address Space */
		case 0x4018 ... 0xFFFF:
			b->m_rom->Write( em, addr, v );
			break;
	}
}

static void BUS_Tick( MOS6502* em )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;
	
	(*(uint64_t*)&b->m_ticks)++;
	b->m_ppu->Tick( em );
	b->m_ppu->Tick( em );
	b->m_ppu->Tick( em );
}

struct NES_BUS* NesEx_CreateBUS( struct NES_ROM* rom, struct NES_PPU* ppu, struct NES_APU* apu )
{
	NES_BUS* r = (NES_BUS*)malloc( sizeof( NES_BUS ) );

	r->Read  = BUS_Read;
	r->Write = BUS_Write;
	r->Tick  = BUS_Tick;

	for( int i = 0; i < 0x800; i++ )
		r->m_ram[ i ] = (i & 4) ? 0xFF : 0x00;
		
	r->m_rom = rom;
	r->m_ppu = ppu;
	r->m_apu = apu;
	
	return r;
}

#ifdef __cplusplus
}
#endif
