#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "..//include//rom.h"
#include "..//include//map.h"
#include "..//include//bus.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t ROM_Read( struct MOS6502* em, uint16_t addr )
{
	struct NES_BUS* b = (struct NES_BUS*)em->m_bus;
	struct NES_ROM* r = b->m_rom;

	r->m_map->Read( em, addr );
	switch( addr )
	{
	case 0x8000 ... 0xBFFF:
		return r->m_map->m_selectedPROM[0][ addr & 0x3FFF ];
	case 0xC000 ... 0xFFFF:
		return r->m_map->m_selectedPROM[1][ addr & 0x3FFF ];
	default:
		fprintf( stderr, "[ ROM ] Read() : Unhandled read, addr : 0x%X\n", addr );
		return 0;
	}
}

void   ROM_Write( struct MOS6502* em, uint16_t addr, uint8_t v )
{
	struct NES_BUS* b = (struct NES_BUS*)em->m_bus;
	struct NES_ROM* r = b->m_rom;

	r->m_map->Write( em, addr, v );
	switch( addr )
	{
	case 0x8000 ... 0xBFFF:
		r->m_map->m_selectedPROM[0][ addr & 0x3FFF ] = v; break;
	case 0xC000 ... 0xFFFF:
		r->m_map->m_selectedPROM[1][ addr & 0x3FFF ] = v; break;
	default:
		fprintf( stderr, "[ ROM ] Write() : Unhandled write, addr : 0x%X | v : 0x%X\n", addr, v );
		break;
	}
}

NES_ROM* NesEx_CreateROM( char * fileDir )
{
	FILE *fd = fopen( fileDir, "rd" );
	if( !fd ) 
	{
		fprintf( stderr, "[ Error ]  %s\n", strerror( errno ) );
		return NULL;
	}

	NES_ROM*   nf = (NES_ROM*)   calloc( 1, sizeof( NES_ROM ) );
	nf->m_header  = (NES_HEADER*)calloc( 1, sizeof( NES_HEADER ) );

	nf->Read      = ROM_Read;
	nf->Write     = ROM_Write;

	if( fread( (void*)nf->m_header, NES_HEADER_SIZE , 1, fd ) != 1 )
	{
		free( nf->m_header );
		free( nf );
		fprintf( stderr, "[ Error ] read(), Cannot load NES header to memory.\n " );
		return NULL;
	}
	
	fprintf( stdout, "ROM MapperID: %d\n", nf->m_header->m_lowerMapperNybble | (uint8_t)(nf->m_header->m_upperMapperNybble << 4) );
	fprintf( stdout, "PROM %d * 16kb\n", nf->m_header->m_PROMBankCount );
	fprintf( stdout, "CROM %d *  8kb\n", nf->m_header->m_CROMBankCount );
	fprintf( stdout, "PRAM %d *  8kb\n", nf->m_header->m_pramSize ); 
	fprintf( stdout, "BB : %s\n", nf->m_header->m_batteryBacked ? "true" : "false" ); 


	if( nf->m_header->m_nes[0] != 'N' || 
		nf->m_header->m_nes[1] != 'E' ||
		nf->m_header->m_nes[2] != 'S' ||  
		nf->m_header->m_nes[3] != 0x1A ) 
	{
		free( nf->m_header );
		free( nf );
		fprintf( stderr, "[ Error ] Invalid NES header format.\n" );
		return NULL;
	}

	nf->m_PROM = (uint8_t**)realloc( nf->m_PROM, sizeof(uint8_t*) * ( nf->m_header->m_PROMBankCount) );
	nf->m_CROM = (uint8_t**)realloc( nf->m_CROM, sizeof(uint8_t*) * ( nf->m_header->m_CROMBankCount) );
	
	if( nf->m_header->m_trainer )
	{
		nf->m_trainer = (uint8_t*)malloc( NES_TRAINER_SIZE );
		if( fread( (void*)nf->m_trainer, NES_TRAINER_SIZE, 1, fd ) != 1 )
		{
			free( nf->m_trainer );
			free( nf->m_header );
			free( nf );
			fprintf( stderr, "[ Error : %s ] fread(), Trainer ROM : Corrupt ROM file. \n", strerror( errno ) );
			return NULL;
		}
	}

	for( unsigned int iter = 0; iter <  nf->m_header->m_PROMBankCount; iter++ ) 
	{
		nf->m_PROM[iter] = (uint8_t*)malloc( NES_PROM_BANK_SIZE );
		if( fread( (void*)nf->m_PROM[iter], NES_PROM_BANK_SIZE, 1, fd ) != 1 )
		{
			free( nf->m_trainer );
			free( nf->m_header );
			free( nf );
			fprintf( stderr, "[ Error : %s ] fread(), Program ROM : Corrupt ROM file. \n", strerror( errno ) );
		}
	}

	for( unsigned int iter = 0; iter < nf->m_header->m_CROMBankCount; iter++ ) 
	{
		nf->m_CROM[iter] = (uint8_t*)malloc( NES_CROM_BANK_SIZE );
		if( fread( (void*)nf->m_CROM[iter], NES_CROM_BANK_SIZE, 1, fd ) != 1 )
		{
			free( nf->m_trainer );
			free( nf->m_header );
			free( nf );
			fprintf( stderr, "[ Error : %s ] fread(), Character ROM : Corrupt ROM file. \n", strerror( errno ) );
		}
	}
	fclose( fd );
	
	nf->m_map = NesEx_CreateMAP( nf );

	return nf;
}

#ifdef __cplusplus
}
#endif
