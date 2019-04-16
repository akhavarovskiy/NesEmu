#include "..//include//nesbus.h"
#include "..//include//nesrom.h"

u8 NESReadMemory( MOS6502* em, u16 addr )
{
	MOS6502_TEST_BUS* bus = (MOS6502_TEST_BUS*)em->m_bus;
	NES_FILE* nf = (NES_FILE*)bus->m_ptr[1];

	int v = 0;
	switch( addr )
	{
	case 0x0000 ... 0x1FFF: // RAM
		return ((u8*)bus->m_ptr[0])[ addr & 0x7FF ];

	case 0x2000 ... 0x3FFF: // PPU
		return 0;
		
	case 0x4000 ... 0x4017: // APU & IO
		return 0;

	case 0x4018 ... 0xFFFF: // ROM
		switch( addr )
		{
			case 0x8000 ... 0xBFFF:
				return nf->m_ROM[ nf->m_selectedBank[0] ][ addr % 0x4000 ];

			case 0xC000 ... 0xFFFF:
				return nf->m_ROM[ nf->m_selectedBank[1] ][ addr % 0x4000 ];
		};
		break;
	}
}

void NESWriteMemory( MOS6502* em, u16 addr, u8 v )
{
	MOS6502_TEST_BUS* bus = (MOS6502_TEST_BUS*)em->m_bus;

	switch( addr )
	{
	case 0x0000 ... 0x1FFF: // RAM
		((u8*)bus->m_ptr[0])[ addr & 0x7FF ] = v;
		break;

	case 0x2000 ... 0x3FFF: // PPU
		break;
		
	case 0x4000 ... 0x4017: // APU & IO
		break;

	case 0x4018 ... 0xFFFF: // ROM
		switch( addr )
		{
			case 0x8000 ... 0xBFFF:
				((NES_FILE*)(bus->m_ptr[2]))->m_ROM[ ((NES_FILE*)bus->m_ptr[2])->m_selectedBank[0] ][ addr % 0x4000 ] = v;
			case 0xC000 ... 0xFFFF:
				((NES_FILE*)(bus->m_ptr[2]))->m_ROM[ ((NES_FILE*)bus->m_ptr[2])->m_selectedBank[1] ][ addr % 0x4000 ] = v;
		};
		break;
	}
};

void NESTick( MOS6502* em ) 
{
	(*(int*)(&em->m_bus->m_ticks))++;
};

MOS6502_TEST_BUS* CreateNesBus( void )
{
	MOS6502_TEST_BUS* b = (MOS6502_TEST_BUS*)malloc( sizeof( MOS6502_TEST_BUS ) );

	b->Read 	   = NESReadMemory;
	b->Write 	   = NESWriteMemory;
	b->Tick 	   = NESTick;

	b->m_ptr 	   = (const void**)malloc( sizeof( void* ) * 2 );
	b->m_ptr[0]    = (u8*   )malloc( sizeof( u8    ) * 0x3000 ); // RAM
	b->m_ptr[1]    = (NES_FILE*)LoadROM( "nestest.nes" );

	return b;
};
