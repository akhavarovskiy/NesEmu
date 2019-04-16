#include <stdlib.h>
#include <assert.h>
#include "..//include//6502.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "6502Core.c"

/*=================================================================
	Run()
===================================================================
	*this_ptr, 
*/
static MOS6502_INS* Run( struct MOS6502* this_ptr )
{
	if( this_ptr->m_int.RST_TRIGGER )
	{
		this_ptr->m_int.RST( this_ptr );
		this_ptr->m_int.RST_TRIGGER = false;
	}
	else
	if( this_ptr->m_int.NMI_TRIGGER  && !this_ptr->m_int.NMI_TRIGGER_EDGE )
	{
		this_ptr->m_int.NMI( this_ptr );
		this_ptr->m_int.NMI_TRIGGER_EDGE = true;
	}
	else
	if( this_ptr->m_int.IRQ_TRIGGER && !this_ptr->m_reg.PS.I )
	{
		this_ptr->m_int.IRQ( this_ptr );
	}
	if( !this_ptr->m_int.NMI_TRIGGER )
	{
		this_ptr->m_int.NMI_TRIGGER_EDGE = false;
	}
	uint8_t lvOpCode = this_ptr->m_bus->Read( this_ptr, this_ptr->m_reg.PC++ ) ;
	MOS6502_INS* i = 
		&g_instructionSet[ lvOpCode ];

	if( i->Func ) {
		i->Func( i, this_ptr );
		return i;
	}
	fprintf( stdout, "[ MOS6502 ] Run(), Unknown opcode : 0x%X | PC : 0x%X\n", lvOpCode, this_ptr->m_reg.PC );
	return NULL;
};


/*=================================================================
	CreateMachine()
===================================================================
	*bus -> Requires a memory bus for a given machine to operate. */

MOS6502* CreateMachine( struct MOS6502_BUS* bus )
{
	assert( "[ MOS6502 ] Create6502Machine(), Need a valid memory bus to function." && bus );

	MOS6502 *r = (MOS6502*)calloc( 1, sizeof( MOS6502 ) );

	/* Copy the memory bus pointer */
	r->Run             = Run;
	r->m_bus           = bus;

	r->m_int.IRQ       = IRQ;
	r->m_int.NMI       = NMI;
	r->m_int.RST       = RST;

	r->m_int.RST_TRIGGER  = false;
	r->m_int.IRQ_TRIGGER  = false;
	r->m_int.NMI_TRIGGER  = false;
	r->m_int.NMI_TRIGGER_EDGE  = false;

	/* Read the reset vector and set the program counter. */
	r->m_int.RST( r );

	return r;
}

/*=================================================================
	DestroyMachine()
===================================================================
	*em -> Machine to destroy. */

void DestroyMachine( struct MOS6502* em )
{
	free( em );
	em = NULL;
}

#ifdef __cplusplus
}
#endif