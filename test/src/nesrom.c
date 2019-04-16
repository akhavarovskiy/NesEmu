#include "..//include//nesrom.h"

NES_FILE* LoadROM( char* file )
{
	FILE *fd = fopen( file, "rd" );
	if( !fd ) 
	{
		fprintf( stderr, "[ Error ] %s\n", strerror( errno ) );
		return NULL;
	}
	int sz = \
	fseek( fd, 0, SEEK_END );
	fseek( fd, 0, SEEK_SET );

	NES_FILE * nf = (NES_FILE  *)calloc( 1, sizeof( NES_FILE   ) ); 
	nf->m_header  = (NES_HEADER*)malloc(    sizeof( NES_HEADER ) );

	if( fread( (void*)nf->m_header, NES_HEADER_SIZE , 1, fd ) != 1 )
	{
		free( nf->m_header );
		free( nf );
		fprintf( stderr, "[ Error ] read(), Cannot load NES header to memory.\n " );
		return NULL;
	}

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

	nf->m_ROM  = (u8**)malloc( sizeof(u8*) * nf->m_header->m_programRomSize   );
	nf->m_CROM = (u8**)malloc( sizeof(u8*) * nf->m_header->m_characterRomSize );
	
	if( nf->m_header->m_traniner )
	{
		nf->m_traniner = (u8*)malloc( NES_TRAINER_SIZE );
		if( fread( (void*)nf->m_traniner, NES_TRAINER_SIZE, 1, fd ) != 1 )
		{
			free( nf->m_traniner );
			free( nf->m_header );
			free( nf );
			fprintf( stderr, "[ Error : %s ] fread(), Trainer ROM : Corrupt ROM file. \n", strerror( errno ) );
			return NULL;
		}
	}

	for( unsigned int iter = 0; iter <  nf->m_header->m_programRomSize; iter++ ) 
	{
		nf->m_ROM[iter] = (u8*)malloc( NES_ROM_BANK_SIZE );
		if( fread( (void*)nf->m_ROM[iter], NES_ROM_BANK_SIZE, 1, fd ) != 1 )
		{
			free( nf->m_traniner );
			free( nf->m_header );
			free( nf );
			fprintf( stderr, "[ Error : %s ] fread(), Program ROM : Corrupt ROM file. \n", strerror( errno ) );
		}
	}

	for( unsigned int iter = 0; iter <  nf->m_header->m_characterRomSize; iter++ ) 
	{
		nf->m_CROM[iter] = (u8*)malloc( NES_CROM_BANK_SIZE );
		if( fread( (void*)nf->m_CROM[iter], NES_CROM_BANK_SIZE, 1, fd ) != 1 )
		{
			free( nf->m_traniner );
			free( nf->m_header );
			free( nf );
			fprintf( stderr, "[ Error : %s ] fread(), Character ROM : Corrupt ROM file. \n", strerror( errno ) );
		}
	}
	fclose( fd );

	nf->m_mapperId = ((u8)nf->m_header->m_upperMapperNybble << 4) | (u8)nf->m_header->m_lowerMapperNybble;

	switch( nf->m_mapperId )
	{
	case 0x00:
		nf->m_selectedBank[0]      = 0;
		nf->m_selectedBank[1]      = 1;
		nf->m_selectedCharacterROM = 0;

		if( nf->m_header->m_programRomSize == 1 )
			nf->m_selectedBank[0] = nf->m_selectedBank[1] = 0;
		break;
	case 0x01:
		nf->m_selectedBank[0]      = 0;
		nf->m_selectedBank[1]      = 1;
		nf->m_selectedCharacterROM = 0;
		break;
	
	case 0x02:
		nf->m_selectedBank[0]      = 0;
		nf->m_selectedBank[1]      = nf->m_header->m_programRomSize - 1;
		nf->m_selectedCharacterROM = 0;
		nf->m_CROM      = (u8**)malloc( sizeof( u8**) );
		nf->m_CROM[ 0 ] = (u8* )calloc( 1, NES_CROM_BANK_SIZE );
		break;
	case 0x03:
		nf->m_selectedBank[0]      = 0;
		nf->m_selectedBank[1]      = 1;
		nf->m_selectedCharacterROM = 0;
		break;
	
	case 0x09:
		nf->m_selectedBank[0]      = 6;
		nf->m_selectedBank[1]      = 7;
		nf->m_selectedCharacterROM = 0; 
		break;

	case 0x42:
		nf->m_selectedCharacterROM = 1;
		nf->m_selectedBank[0] = 3;
		nf->m_selectedBank[1] = 3;
		break;
	}
	return nf;
}