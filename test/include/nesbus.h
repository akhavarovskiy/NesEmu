#ifndef __NES_BUS_H__
#define __NES_BUS_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <6502.h>


typedef struct MOS6502_TEST_BUS
{
	/* Perform external operations based on CPU execution. */
	void    (*Tick ) ( struct MOS6502* em );

	/* Read Write to the CPU memory space. */
	uint8_t (*Read ) ( struct MOS6502* em, uint16_t addr );
	void    (*Write) ( struct MOS6502* em, uint16_t addr, uint8_t v );

 	/* Counts the amount of ticks that have occured. */
	const uint64_t m_ticks;

	const void** m_ptr;
} 
MOS6502_TEST_BUS;

MOS6502_TEST_BUS* CreateNesBus( void );

#endif