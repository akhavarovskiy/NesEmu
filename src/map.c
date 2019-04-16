#include <6502.h>
#include <stdio.h>
#include <stdlib.h>

#include "..//include//map.h"
#include "..//include//bus.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MAPPER1_DATA {
	
};

static uint8_t GenericMapperRead( struct MOS6502* em, uint16_t addr )
{
	return 0;
}

static void   GenericMapperWrite( struct MOS6502* em, uint16_t addr, uint8_t v )
{
	fprintf( stdout, "[ Default ] Write() : addr 0x%X @ 0x%X\n", addr, v );
	return;
}

/*
4bit0
-----
CPPMM
|||||
|||++- Mirroring (0: one-screen, lower bank; 1: one-screen, upper bank;
|||               2: vertical; 3: horizontal)
|++--- PRG ROM bank mode (0, 1: switch 32 KB at $8000, ignoring low bit of bank number;
|                         2: fix first bank at $8000 and switch 16 KB bank at $C000;
|                         3: fix last bank at $C000 and switch 16 KB bank at $8000)
+----- CHR ROM bank mode (0: switch 8 KB at a time; 1: switch two separate 4 KB banks)
*/
static void Mapper1_Write( struct MOS6502* em, uint16_t addr, uint8_t v )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;

	static uint8_t MMC1_SR, 
				   MMC1_PB,
				   MMC1_RS;

	if( (v & 0x80) == 0x80 ) {
		MMC1_SR = 0b10000;
		//b->m_rom->m_map->m_selectedPROM[ 0 ] = b->m_rom->m_PROM[ v & 0b111 ];
		fprintf( stdout, "[ Mapper 1 ] Serial Write : addr 0x%X -> 0x%X \n", addr, v );

	}
	else {
		MMC1_SR = ((MMC1_SR >> 1) | ( v & 0x01 ) << 4);
		if( MMC1_SR & 0x01 )
		{
			MMC1_SR = ((MMC1_SR >> 1) | ( v & 0x01 ) << 4 );
			MMC1_PB = MMC1_SR;
			MMC1_SR = 0b10000;

			switch( addr )
			{
				// Control Register
				case 0x8000 ... 0x9FFF:
					fprintf( stdout, "CONTROLL : 0x%X -> 0x%X\n", addr, v );
					break;

				// CBank0 Select
				case 0xA000 ... 0xBFFF:
					b->m_rom->m_map->m_selectedCROM      = b->m_rom->m_CROM[ MMC1_PB ];
					fprintf( stdout, "CHROM Bank0 : 0x%X -> 0x%X\n", addr, MMC1_PB );
					break;

				// CBank1 Select
				case 0xC000 ... 0xDFFF:
					b->m_rom->m_map->m_selectedCROM      = b->m_rom->m_CROM[ MMC1_PB ];
					fprintf( stdout, "CHROM Bank1 : 0x%X -> 0x%X\n", addr, MMC1_PB );
					break;

				// PGR Bank Select
				case 0xE000 ... 0xFFFF:
					b->m_rom->m_map->m_selectedPROM[ 0 ] = b->m_rom->m_PROM[ 0 ];
					fprintf( stdout, "PGR Bank1 : 0x%X -> 0x%X\n", addr, MMC1_PB );
					break;
			};
			//fprintf( stdout, "[ Mapper 1 ] Serial() : addr 0x%X @ 0x%X : PB @ 0x%X RS @ 0x%X\n", addr, v, MMC1_PB,  MMC1_RS );
		}
	}
}

static void Mapper2_Write( struct MOS6502* em, uint16_t addr, uint8_t v )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;
	fprintf( stdout, "[ Mapper 2 ] Write() : addr 0x%X @ 0x%X\n", addr, v );
	b->m_rom->m_map->m_selectedPROM[0] = b->m_rom->m_PROM[ v & 0b111 ];
}

static void Mapper3_Write( struct MOS6502* em, uint16_t addr, uint8_t v )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;
	if( v & 0x80 == 0x80 )
		fprintf( stdout, "[ Mapper 3 ] Serial Write : addr 0x%X @ 0x%X\n",  addr, v  );
	else
		fprintf( stdout, "[ Mapper 3 ] Write() : addr 0x%X @ 0x%X\n", addr, v );
	b->m_rom->m_map->m_selectedCROM    = b->m_rom->m_CROM[ v & 0b11 ];
}

static void Mapper7_Write( struct MOS6502* em, uint16_t addr, uint8_t v )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;
	fprintf( stdout, "[ Mapper 7 ] Write() : addr 0x%X @ 0x%X\n", addr, v );
	b->m_rom->m_map->m_selectedPROM[ 0 ]    = b->m_rom->m_PROM[ v & 0b111 ];
}

static void Mapper66_Write( struct MOS6502* em, uint16_t addr, uint8_t v )
{
	NES_BUS* b = (NES_BUS*)em->m_bus;
	fprintf( stdout, "[ Mapper 66 ] Write() : addr 0x%X @ 0x%X\n", addr, v );

	uint8_t l = ( v >> 4 );

	fprintf( stdout, "[ Mapper 66 ] CC:0x%X\n",   v        & 0b11 );
	fprintf( stdout, "[ Mapper 66 ] PP:0x%X\n", ( v >> 4 ) & 0b11 );

	b->m_rom->m_map->m_selectedPROM[0] = b->m_rom->m_PROM[  l*2      & 0b11 ];
	b->m_rom->m_map->m_selectedPROM[1] = b->m_rom->m_PROM[  l*2+1    & 0b11 ];
	b->m_rom->m_map->m_selectedCROM    = b->m_rom->m_CROM[  v    & 0b11 ];
}


NES_MAP* NesEx_CreateMAP( struct NES_ROM* rom )
{
	NES_MAP* r = (NES_MAP*)malloc( sizeof( NES_MAP ) );

	r->m_rom = rom;
	r->m_id  = rom->m_header->m_lowerMapperNybble | (rom->m_header->m_upperMapperNybble << 4);

	switch( r->m_id )
	{
	case 0:
		r->Read  = GenericMapperRead;
		r->Write = GenericMapperWrite;

		r->m_selectedPROM[0] = rom->m_PROM[ 0 ];
		r->m_selectedPROM[1] = rom->m_PROM[ (rom->m_header->m_PROMBankCount == 1) ? 0 : 1 ];
		r->m_selectedCROM    = rom->m_CROM[ 0 ];
		break;
	case 1:
		r->Read  = GenericMapperRead;
		r->Write = Mapper1_Write;

		r->m_selectedPROM[0] = rom->m_PROM[ 0 ];
		r->m_selectedPROM[1] = rom->m_PROM[ rom->m_header->m_PROMBankCount - 1 ];
		r->m_selectedCROM    = rom->m_CROM[ 0 ];
		break;

	case 2:
		r->Read  = GenericMapperRead;
		r->Write = Mapper2_Write;

		rom->m_CROM    = (uint8_t**)realloc( rom->m_CROM, sizeof( void**) );
		rom->m_CROM[0] = (uint8_t* )malloc( NES_CROM_BANK_SIZE );

		r->m_selectedPROM[0] = rom->m_PROM[ 0 ];
		r->m_selectedPROM[1] = rom->m_PROM[ rom->m_header->m_PROMBankCount - 1 ];
		r->m_selectedCROM    = rom->m_CROM[ 0 ];
		break;
	
	case 3:
		r->Read  = GenericMapperRead;
		r->Write = Mapper3_Write;

		r->m_selectedPROM[0] = rom->m_PROM[ 0 ];
		r->m_selectedPROM[1] = rom->m_PROM[ (rom->m_header->m_PROMBankCount == 1) ? 0 : 1 ];
		r->m_selectedCROM    = rom->m_CROM[ 0 ];
		break;
	
	case 7:
		r->Read  = GenericMapperRead;
		r->Write = Mapper7_Write;

		r->m_selectedPROM[0] = rom->m_PROM[ 6 ];
		r->m_selectedPROM[1] = rom->m_PROM[ 7 ];
		r->m_selectedCROM    = (uint8_t*)malloc( 0x2000 );

	case 66:
		r->Read  = GenericMapperRead;
		r->Write = Mapper66_Write;

		r->m_selectedPROM[0] = rom->m_PROM[ 0 ];
		r->m_selectedPROM[1] = rom->m_PROM[ 1 ];
		r->m_selectedCROM    = rom->m_CROM[ 0 ];
		break;
	}
	return r;
}

#ifdef __cplusplus
}
#endif
