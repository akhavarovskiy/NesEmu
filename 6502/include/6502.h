#ifndef MOS6502_H__
#define MOS6502_H__

/*=======================================================================
	6502.h :
	* Aleksandr Khavarovskiy
	* Washington State University - Vancouver
	======================================================================
	* Designed to be reusable for emulation of systems using the 6502.
*/
#include <stdint.h>
#include <stdbool.h>

/* Vector Address's */
#define NMI_ADDRESS 0xFFFA
#define RST_ADDRESS 0xFFFC
#define IRQ_ADDRESS 0xFFFE

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MOS6502 MOS6502;
/*===================================================================
	MOS6502 Registers
=====================================================================
	* Registers used by the MOS6502 CPU */

typedef struct MOS6502_REG
{
	union PSR
	{
		uint8_t m_data;
		struct
		{
			unsigned C : 1; // Carry
			unsigned Z : 1; // Zero
			unsigned I : 1; // Interrupt
			unsigned D : 1; // Decimal
			unsigned B : 1; // Break
			unsigned P : 1; // Not Used
			unsigned V : 1; // Overflow
			unsigned N : 1; // Negative
		};
	}        PS;
	uint16_t PC;
	uint8_t   A;
	uint8_t   X;	
	uint8_t   Y;
	uint8_t  SP;
}
MOS6502_REG;

/*===================================================================
	MOS6502 Interrupts
=====================================================================
	* All the interrupt logic is handled by the folowing structure. */

typedef struct MOS6502_INT
{
	void (*IRQ)( struct MOS6502* this_ptr );
	void (*NMI)( struct MOS6502* this_ptr );
	void (*RST)( struct MOS6502* this_ptr );

	bool IRQ_TRIGGER;
	bool NMI_TRIGGER;
	bool NMI_TRIGGER_EDGE;
	bool RST_TRIGGER;
} 
MOS6502_INT;

/*=================================================================
	MOS6502 Instruction information
===================================================================
	*Pertinant information of a given instruction. */

typedef struct MOS6502_INS
{
	void     (*Func)( struct MOS6502_INS* this_ptr, struct MOS6502* em ); // Instruction function
	uint16_t (*Mode)( struct MOS6502_INS* this_ptr, struct MOS6502* em ); // Memory mode function

	const char*  m_name;          // Name of instruction, for debuging 
	unsigned int m_clock;         // Base clock
	bool         m_boundary;      // Execute an additional tick on boundary cross.
} 
MOS6502_INS;

/*===================================================================
	MOS6502 Memory BUS
=====================================================================
	* Structure used for interproccess communication. */

typedef struct MOS6502_BUS
{
	void    (*Tick ) ( struct MOS6502* em );
	uint8_t (*Read ) ( struct MOS6502* em, uint16_t addr );
	void    (*Write) ( struct MOS6502* em, uint16_t addr, uint8_t v );

	/* Counts the amount of ticks that have occurred. */
	const uint64_t m_ticks;
} 
MOS6502_BUS;

/*=================================================================
	MOS6502
===================================================================
	*Will run an instance of the 6502 CPU given a systems memory bus.
*/
typedef struct MOS6502
{
  /*===============================================================
	Run()
	===============================================================
	- Returns : the information about the last executed instruction*/
  /*===============================================================*/
	MOS6502_INS* (*Run)( struct MOS6502* this_ptr );
  /*===============================================================*/
	MOS6502_BUS* m_bus;
	MOS6502_REG  m_reg;
	MOS6502_INT  m_int;
}
MOS6502;

/*=================================================================
	CreateMachine()
===================================================================
	*bus -> Requires a memory bus for a given machine to operate. */
	
MOS6502* CreateMachine( MOS6502_BUS* bus );


/*=================================================================
	DestroyMachine()
===================================================================
	*em -> Machine to destroy. */
void DestroyMachine( MOS6502*  em );


#ifdef __cplusplus
}
#endif

#endif