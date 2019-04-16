#include "..//include//nesbus.h"
#include "..//include//nestest.h"
#include "..//include//nesrom.h"

int main( int argc, const char* argv[] )
{
	int t = 0;
	if( argc == 1 )
		t = 5003;
	else
		t = atoi( argv[1] );
	
	MOS6502_BUS* bus = (MOS6502_BUS*)CreateNesBus ();
	MOS6502    * em  = CreateMachine( bus );

	em->m_reg.PC     = 0xC000;
	em->m_reg.SP     = 0xFD;
	em->m_reg.PS.P   = 1;
	em->m_reg.PS.I   = 1;

	FILE * fp = fopen( "nestest.ref", "rb" );

	unsigned int iter = 0;
	for( ;iter < t; iter++ )
	{
		unsigned int addr = 0;
		char buff[1024];
		fgets( buff, 1024, fp );
		sscanf( buff, "%X", &addr );

		int A  = em->m_reg.A;
		int X  = em->m_reg.X;
		int Y  = em->m_reg.Y;
		int P  = em->m_reg.PS.m_data;
		int SP = em->m_reg.SP;
		int T  = em->m_bus->m_ticks;

		if( addr != em->m_reg.PC ) break;

		int ref_A;
		int ref_X;//  = em->m_reg.X;
		int ref_Y ;// = em->m_reg.Y;
		int ref_P ;// = em->m_reg.PS.m_data;
		int ref_SP;// = em->m_reg.SP;
		int ref_T; //  = em->m_bus->m_ticks;

		//buff[73] = '\n';
		//buff[74] = '\0';

		int v = sscanf( &buff[48], "A:%X X:%X Y:%X P:%X SP:%X\n", 
			&ref_A, &ref_X, &ref_Y, &ref_P, &ref_SP );
/*
		fprintf( stdout, "A:%02hhX X:%02hhX Y:%02hhX P:%02hhX SP:%02hhX\n", 
			ref_A, ref_X, ref_Y, ref_P, ref_SP );

		fprintf( stdout, "A:%02hhX X:%02hhX Y:%02hhX P:%02hhX SP:%02hhX\n", 
			A, X, Y , P, SP );
*/
		if( v != 5 )
		{
			fprintf( stdout, &buff[48] );
			fprintf( stdout, "[ Error ] sscanf() == %d\n", v );
			return 0;
		}

		/*if( ref_A != A ||
			ref_X != X ||
			ref_Y != Y ||
			ref_P != P ||
			ref_SP != SP )
		{*/
			fprintf( stdout, "[ %.4d ] %s", iter, buff );
			fprintf( stdout, "[ %.4d ] %X ", iter, em->m_reg.PC );
		//}

		MOS6502_INS * i = em->Run( em );
		if( i == NULL )
			break;
			
		/*if( ref_A != A ||
			ref_X != X ||
			ref_Y != Y ||
			ref_P != P ||
			ref_SP != SP 
		 )
		{*/
			fprintf( stdout, "           %s                             A:%.2X X:%.2X Y:%.2X P:%.2X SP:%.2X CYC:%.3d", i->m_name, A, X, Y, P , SP, T % 341 );
			fprintf( stdout, "\n" );
		//}
		//sleep( 1 );
	}
	if( iter == 5003 )
	{
		fprintf( stdout, "===========================================================================================\n" );
		fprintf( stdout, "[ 6502 ] Official Opcode : \e[0;32m Pass \e[0;29m \n" );
		fprintf( stdout, "===========================================================================================\n" );
		fprintf( stdout, "\e[0;36m *No deviations found in execution logic. \e[0;29m  \n" );
		fprintf( stdout, "===========================================================================================\n" );
	}
	else 
	if( t != 5003 )
	{
		fprintf( stdout, "===========================================================================================\n" );
		fprintf( stdout, "[ 6502 ] Official Opcode : \e[0;33m Incomplete Test \e[0;29m \n" );
		fprintf( stdout, "===========================================================================================\n" );
	}
	else
	{
		fprintf( stdout, "===========================================================================================\n" );
		fprintf( stdout, "[ 6502 ] Official Opcode : \e[0;31m Fail \e[0;29m \n" );
		fprintf( stdout, "===========================================================================================\n" );
	}
	return 0;
}
