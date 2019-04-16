#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "..//include//6502.h"

#define MOS6502_ADC_IMMEDIATE   0x69
#define MOS6502_AND_IMMEDIATE   0x29  
#define MOS6502_CMP_IMMEDIATE   0xC9 
#define MOS6502_CPY_IMMEDIATE   0xC0 
#define MOS6502_CPX_IMMEDIATE   0xE0 
#define MOS6502_EOR_IMMEDIATE   0x49 
#define MOS6502_LDA_IMMEDIATE   0xA9 
#define MOS6502_LDX_IMMEDIATE   0xA2 
#define MOS6502_LDY_IMMEDIATE   0xA0 
#define MOS6502_ORA_IMMEDIATE   0x09 
#define MOS6502_SBC_IMMEDIATE   0xE9 

#define MOS6502_ASL_ACCUMULATOR 0x0A
#define MOS6502_LSR_ACCUMULATOR 0x4A
#define MOS6502_ROL_ACCUMULATOR 0x2A
#define MOS6502_ROR_ACCUMULATOR 0x6A

#define MOS6502_ADC_ZEROPAGE    0x65
#define MOS6502_AND_ZEROPAGE    0x25
#define MOS6502_ASL_ZEROPAGE    0x06
#define MOS6502_BIT_ZEROPAGE    0x24
#define MOS6502_CMP_ZEROPAGE    0xC5
#define MOS6502_CPX_ZEROPAGE    0xE4
#define MOS6502_CPY_ZEROPAGE    0xC4
#define MOS6502_EOR_ZEROPAGE    0x45
#define MOS6502_LDA_ZEROPAGE    0xA5
#define MOS6502_LDX_ZEROPAGE    0xA6
#define MOS6502_LDY_ZEROPAGE    0xA4
#define MOS6502_ORA_ZEROPAGE    0x05
#define MOS6502_SBC_ZEROPAGE    0xE5
#define MOS6502_STA_ZEROPAGE    0x85
#define MOS6502_STX_ZEROPAGE    0x86
#define MOS6502_STY_ZEROPAGE    0x84
#define MOS6502_DEC_ZEROPAGE    0xC6
#define MOS6502_INC_ZEROPAGE    0xE6
#define MOS6502_LSR_ZEROPAGE    0x46
#define MOS6502_ROL_ZEROPAGE    0x26
#define MOS6502_ROR_ZEROPAGE    0x66

#define MOS6502_ADC_ZEROPAGEX   0x75
#define MOS6502_AND_ZEROPAGEX   0x35
#define MOS6502_CMP_ZEROPAGEX   0xD5
#define MOS6502_EOR_ZEROPAGEX   0x55
#define MOS6502_LDA_ZEROPAGEX   0xB5
#define MOS6502_LDY_ZEROPAGEX   0xB4
#define MOS6502_ORA_ZEROPAGEX   0x15
#define MOS6502_SBC_ZEROPAGEX   0xF5
#define MOS6502_STA_ZEROPAGEX   0x95
#define MOS6502_STY_ZEROPAGEX   0x94
#define MOS6502_ASL_ZEROPAGEX   0x16
#define MOS6502_DEC_ZEROPAGEX   0xD6
#define MOS6502_INC_ZEROPAGEX   0xF6
#define MOS6502_LSR_ZEROPAGEX   0x56
#define MOS6502_ROL_ZEROPAGEX   0x36
#define MOS6502_ROR_ZEROPAGEX   0x76

#define MOS6502_LDX_ZEROPAGEY   0xB6 
#define MOS6502_STX_ZEROPAGEY   0x96 

#define MOS6502_ADC_ABSOLUTE    0x6D
#define MOS6502_AND_ABSOLUTE    0x2D
#define MOS6502_BIT_ABSOLUTE    0x2C
#define MOS6502_CMP_ABSOLUTE    0xCD
#define MOS6502_CPX_ABSOLUTE    0xEC
#define MOS6502_CPY_ABSOLUTE    0xCC
#define MOS6502_EOR_ABSOLUTE    0x4D
#define MOS6502_JMP_ABSOLUTE    0x4C
#define MOS6502_LDA_ABSOLUTE    0xAD
#define MOS6502_LDX_ABSOLUTE    0xAE
#define MOS6502_LDY_ABSOLUTE    0xAC
#define MOS6502_ORA_ABSOLUTE    0x0D
#define MOS6502_SBC_ABSOLUTE    0xED
#define MOS6502_STA_ABSOLUTE    0x8D
#define MOS6502_STX_ABSOLUTE    0x8E
#define MOS6502_STY_ABSOLUTE    0x8C    
#define MOS6502_ASL_ABSOLUTE    0x0E
#define MOS6502_DEC_ABSOLUTE    0xCE
#define MOS6502_INC_ABSOLUTE    0xEE
#define MOS6502_LSR_ABSOLUTE    0x4E
#define MOS6502_ROL_ABSOLUTE    0x2E
#define MOS6502_ROR_ABSOLUTE    0x6E
#define MOS6502_JSR_ABSOLUTE    0x20

#define MOS6502_ASC_ABSOLUTEX   0x7D
#define MOS6502_AND_ABSOLUTEX   0x3D
#define MOS6502_CMP_ABSOLUTEX   0xDD
#define MOS6502_EOR_ABSOLUTEX   0x5D
#define MOS6502_LDA_ABSOLUTEX   0xBD
#define MOS6502_LDY_ABSOLUTEX   0xBC
#define MOS6502_ORA_ABSOLUTEX   0x1D
#define MOS6502_SBC_ABSOLUTEX   0xFD 
#define MOS6502_STA_ABSOLUTEX   0x9D
#define MOS6502_ASL_ABSOLUTEX   0x1E
#define MOS6502_DEC_ABSOLUTEX   0xDE
#define MOS6502_INC_ABSOLUTEX   0xFE
#define MOS6502_LSR_ABSOLUTEX   0x5E
#define MOS6502_ROL_ABSOLUTEX   0x3E
#define MOS6502_ROR_ABSOLUTEX   0x7E

#define MOS6502_ADC_ABSOLUTEY   0x79
#define MOS6502_AND_ABSOLUTEY   0x39
#define MOS6502_CMP_ABSOLUTEY   0xD9
#define MOS6502_EOR_ABSOLUTEY   0x59
#define MOS6502_LDA_ABSOLUTEY   0xB9
#define MOS6502_LDX_ABSOLUTEY   0xBE
#define MOS6502_ORA_ABSOLUTEY   0x19
#define MOS6502_SBC_ABSOLUTEY   0xF9
#define MOS6502_STA_ABSOLUTEY   0x99

#define MOS6502_BCC_RELATIVE    0x90
#define MOS6502_BCS_RELATIVE    0xB0
#define MOS6502_BEQ_RELATIVE    0xF0
#define MOS6502_BMI_RELATIVE    0x30
#define MOS6502_BNE_RELATIVE    0xD0
#define MOS6502_BPL_RELATIVE    0x10
#define MOS6502_BVC_RELATIVE    0x50
#define MOS6502_BVS_RELATIVE    0x70
#define MOS6502_JMP_INDIRECT    0x6C

#define MOS6502_ADC_INDIRECTX   0x61
#define MOS6502_AND_INDIRECTX   0x21
#define MOS6502_CMP_INDIRECTX   0xC1
#define MOS6502_EOR_INDIRECTX   0x41
#define MOS6502_LDA_INDIRECTX   0xA1
#define MOS6502_ORA_INDIRECTX   0x01
#define MOS6502_SBC_INDIRECTX   0xE1
#define MOS6502_STA_INDIRECTX   0x81

#define MOS6502_AND_INDIRECTY   0x31
#define MOS6502_ADX_INDIRECTY   0x71
#define MOS6502_CMP_INDIRECTY   0xD1
#define MOS6502_EOR_INDIRECTY   0x51
#define MOS6502_LDA_INDIRECTY   0xB1
#define MOS6502_ORA_INDIRECTY   0x11
#define MOS6502_SBC_INDIRECTY   0xF1
#define MOS6502_STA_INDIRECTY   0x91

#define MOS6502_BRK_IMPLIED     0x00
#define MOS6502_CLC_IMPLIED     0x18 
#define MOS6502_CLD_IMPLIED     0xD8  
#define MOS6502_CLI_IMPLIED     0x58 
#define MOS6502_CLV_IMPLIED     0xB8 
#define MOS6502_DEX_IMPLIED     0xCA 
#define MOS6502_DEY_IMPLIED     0x88 
#define MOS6502_INX_IMPLIED     0xE8 
#define MOS6502_INY_IMPLIED     0xC8 
#define MOS6502_NOP_IMPLIED     0xEA 
#define MOS6502_SEC_IMPLIED     0x38 
#define MOS6502_SED_IMPLIED     0xF8 
#define MOS6502_SEI_IMPLIED     0x78 
#define MOS6502_TAX_IMPLIED     0xAA 
#define MOS6502_TAY_IMPLIED     0xA8 
#define MOS6502_TSX_IMPLIED     0xBA 
#define MOS6502_TXA_IMPLIED     0x8A 
#define MOS6502_TXS_IMPLIED     0x9A 
#define MOS6502_TYA_IMPLIED     0x98 
#define MOS6502_PHA_IMPLIED     0x48
#define MOS6502_PHP_IMPLIED     0x08     
#define MOS6502_PLA_IMPLIED     0x68 
#define MOS6502_PLP_IMPLIED     0x28 
#define MOS6502_RTI_IMPLIED     0x40
#define MOS6502_RTS_IMPLIED     0x60

#define NES_NTSC_CLOCK_SPEED_MHZ               1.789773       /* MHZ */
#define NES_NTSC_CLOCK_SPEED_HZ                1789773.0      /*  HZ */
#define NES_PAL_CLOCK_SPEED_MHZ                1.662607       /* MHZ */
#define NES_PAL_CLOCK_SPEED_HZ                 1662607.0      /*  HZ */

#define MOS6502_CLOCK_SPEED_HZ                 NES_NTSC_CLOCK_SPEED_HZ                 /* HZ */
#define MOS6502_SECONDS_PER_CLOCK_CYCLE        1.0/MOS6502_CLOCK_SPEED_HZ              /* SEC */
#define MOS6502_MILLISECONDS_PER_CLOCK_CYCLE   MOS6502_SECONDS_PER_CLOCK_CYCLE*1000.0  /* MIL SEC */

#define IMPLIED     NULL
#define ACCUMULATOR NULL

/* Vector Address's */
#define NMI_ADDRESS 0xFFFA
#define RST_ADDRESS 0xFFFC
#define IRQ_ADDRESS 0xFFFE

/* Defining shorthand signed/unsigned integer of varying size */
typedef int8_t   s8; typedef uint8_t   u8;
typedef int16_t s16; typedef uint16_t u16;
typedef int32_t s32; typedef uint32_t u32;
typedef int64_t s64; typedef uint64_t u64;

//_attribute__((regparm(2)))
#define FUNC_OPT __attribute__((regparm(2))) inline

FUNC_OPT
static u16 IMMEDIATE( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	return em->m_reg.PC++;
}

FUNC_OPT
static u16 ZEROPAGE( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	return em->m_bus->Read( em, em->m_reg.PC++ );
}

FUNC_OPT
static u16 ZEROPAGEX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	return (em->m_bus->Read( em, em->m_reg.PC++ ) + em->m_reg.X) & 0xFF;
}

FUNC_OPT
static u16 ZEROPAGEY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	return (em->m_bus->Read( em, em->m_reg.PC++ ) + em->m_reg.Y) & 0xFF;
}

FUNC_OPT
static u16 ABSOLUTE( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	return em->m_bus->Read( em, em->m_reg.PC++ ) | (em->m_bus->Read( em, em->m_reg.PC++ ) << 8);
}

FUNC_OPT
static u16 ABSOLUTEX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 m = em->m_bus->Read( em, em->m_reg.PC++ ) | (em->m_bus->Read( em, em->m_reg.PC++ ) << 8);
	u16 r = m + em->m_reg.X;

	if( this_ptr->m_boundary && (( r ^ m ) & 0xFF00) )
		em->m_bus->Tick( em );

	return r;
}

FUNC_OPT
static u16 ABSOLUTEY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 m = em->m_bus->Read( em, em->m_reg.PC++ ) | (em->m_bus->Read( em, em->m_reg.PC++ ) << 8);
	u16 r = m + em->m_reg.Y;

	if( this_ptr->m_boundary && (( r ^ m ) & 0xFF00) )
		em->m_bus->Tick( em );

	return r;
}

FUNC_OPT
static u16 RELATIVE( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	return (em->m_reg.PC + 1) + (s8)em->m_bus->Read( em, em->m_reg.PC++ );
}

FUNC_OPT
static u16 INDIRECT( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = em->m_bus->Read( em, em->m_reg.PC++ ) | (em->m_bus->Read( em, em->m_reg.PC++ ) << 8);
	u16 p = a & 0xFF00;
	u8  i = a & 0x00FF;
	return em->m_bus->Read( em, a ) | (em->m_bus->Read( em, p | ++i ) << 8);
}

FUNC_OPT
static u16 INDIRECTX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u8  i = em->m_bus->Read( em, em->m_reg.PC++ ) + em->m_reg.X;
	return  em->m_bus->Read( em, i ) | (em->m_bus->Read( em, ++i ) << 8);
}

FUNC_OPT
static u16 INDIRECTY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	struct MOS6502_BUS *b = em->m_bus;
	u8  i = em->m_bus->Read( em, em->m_reg.PC++ );
	u16 m = em->m_bus->Read( em, i ) | (em->m_bus->Read( em, ++i ) << 8);
	u16 r = m + em->m_reg.Y;

	if( this_ptr->m_boundary && (( r ^ m ) & 0xFF00) )
		em->m_bus->Tick( em );

	return r;
}

FUNC_OPT
static void ADC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );
	u16 t = em->m_reg.A + m + em->m_reg.PS.C;
	
	em->m_reg.PS.N =  (t >> 0x07);
	em->m_reg.PS.Z = !(t &  0xFF);
	em->m_reg.PS.C =  (t >> 0x08);
	em->m_reg.PS.V =  ((~(em->m_reg.A ^ m) &
						 (em->m_reg.A ^ t)) >> 0x07);

	em->m_reg.A    =  (t &  0xFF);

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void AND( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );

	em->m_reg.A    =  em->m_reg.A  & m;
	em->m_reg.PS.N =  em->m_reg.A >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.A;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void ASL( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a =  this_ptr->Mode( this_ptr, em );
	u8  m =  em->m_bus->Read( em, a );

	em->m_reg.PS.C =  m >> 0x07;
	m              =  m << 0x01;
	em->m_reg.PS.Z = !m;
	em->m_reg.PS.N =  m >> 0x07;

	em->m_bus->Write( em, a, m );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void ASL_A( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.C =  em->m_reg.A >> 0x07;
	em->m_reg.A    =  em->m_reg.A << 0x01;
	em->m_reg.PS.Z = !em->m_reg.A;
	em->m_reg.PS.N =  em->m_reg.A >> 0x07;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BCC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 r = this_ptr->Mode( this_ptr, em );

	if( !em->m_reg.PS.C ) 
	{
		em->m_bus->Tick( em );
		
		if( this_ptr->m_boundary && ((r ^ em->m_reg.PC ) & 0xFF00) )
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BCS( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 r = this_ptr->Mode( this_ptr, em );

	if( em->m_reg.PS.C ) 
	{
		em->m_bus->Tick( em );
		
		if( this_ptr->m_boundary && ((r ^ em->m_reg.PC) & 0xFF00) )
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BEQ( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 r = this_ptr->Mode( this_ptr, em );

	if( em->m_reg.PS.Z ) 
	{
		em->m_bus->Tick( em );
		
		if( this_ptr->m_boundary && ((r ^ em->m_reg.PC) & 0xFF00) )
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BIT( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a          = this_ptr->Mode( this_ptr, em );
	u8  m          = em->m_bus->Read( em, a );
	em->m_reg.PS.N =  (m >> 0x07);
	em->m_reg.PS.V =  (m >> 0x06); 
	em->m_reg.PS.Z = !(m  & em->m_reg.A);

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BMI( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 r = this_ptr->Mode( this_ptr, em );

	if( em->m_reg.PS.N ) 
	{
		em->m_bus->Tick( em );
		
		if( this_ptr->m_boundary && ((r ^ em->m_reg.PC) & 0xFF00) )
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BNE( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 r = this_ptr->Mode( this_ptr, em );

	if( !em->m_reg.PS.Z ) 
	{
		em->m_bus->Tick( em );
		
		if( this_ptr->m_boundary && ((r ^ em->m_reg.PC) & 0xFF00) )
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BPL( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 r = this_ptr->Mode( this_ptr, em );

	if( !em->m_reg.PS.N ) 
	{
		em->m_bus->Tick( em );
		
		if( this_ptr->m_boundary && ((r ^ em->m_reg.PC ) & 0xFF00) )
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BRK( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PC++;

	em->m_bus->Write(em, em->m_reg.SP-- | 0x100, (u8)(em->m_reg.PC >> 8));
	em->m_bus->Write(em, em->m_reg.SP-- | 0x100, (u8)(em->m_reg.PC));
	
	em->m_reg.PS.B = 1;
	em->m_bus->Write(em, em->m_reg.SP-- | 0x100, em->m_reg.PS.m_data );
	em->m_reg.PS.I = 1;
	em->m_reg.PC   = em->m_bus->Read( em, IRQ_ADDRESS );
	
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BVC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	struct MOS6502_BUS *b = em->m_bus;
	u16          r = this_ptr->Mode( this_ptr, em );

	if( !em->m_reg.PS.V ) 
	{
		em->m_bus->Tick( em );
		
		if( (r ^ em->m_reg.PC ) & 0xFF00 )
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void BVS( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	struct MOS6502_BUS *b = em->m_bus;
	u16          r = this_ptr->Mode( this_ptr, em );

	if( em->m_reg.PS.V ) 
	{
		em->m_bus->Tick( em );
	
		if( this_ptr->m_boundary && ((r ^ em->m_reg.PC ) & 0xFF00 ))
			em->m_bus->Tick( em ); 

		em->m_reg.PC = r;
	}
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void CLC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.C = 0;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void CLI( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.I = 0;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void CLD( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.D = 0;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void CLV( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.V = 0;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void CMP( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u16 t = em->m_reg.A - em->m_bus->Read( em, a );

	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;
	em->m_reg.PS.C =~(t >> 0x08);

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void CPX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u16 t = em->m_reg.X - em->m_bus->Read( em, a );

	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;
	em->m_reg.PS.C =~(t >> 0x08);

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void CPY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u16 t = em->m_reg.Y - em->m_bus->Read( em, a );

	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;
	em->m_reg.PS.C =~(t >> 0x08);

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void DEC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );

	m--;
	em->m_reg.PS.N =  m >> 0x07;
	em->m_reg.PS.Z = !m;

	em->m_bus->Write( em, a, m );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void DEX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.X--;
	em->m_reg.PS.N =  em->m_reg.X >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.X;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void DEY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.Y--;
	em->m_reg.PS.N =  em->m_reg.Y >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.Y;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void EOR( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );
	
	em->m_reg.A    =  em->m_reg.A  ^ m;
	em->m_reg.PS.N =  em->m_reg.A >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.A;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void INC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );

	m++;
	em->m_reg.PS.N =  m >> 0x07;
	em->m_reg.PS.Z = !m;

	em->m_bus->Write( em, a, m );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void INX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.X++;
	em->m_reg.PS.N =  em->m_reg.X >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.X;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void INY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.Y++;
	em->m_reg.PS.N =  em->m_reg.Y >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.Y;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void JMP( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PC = this_ptr->Mode( this_ptr, em );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void JSR( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 r = em->m_reg.PC + 1;

	em->m_bus->Write(em, em->m_reg.SP-- | 0x100, r >> 8);
	em->m_bus->Write(em, em->m_reg.SP-- | 0x100, r);

	em->m_reg.PC = this_ptr->Mode( this_ptr, em );
	
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void LDA( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );

	em->m_reg.A    =  m;
	em->m_reg.PS.N =  m >> 0x07;
	em->m_reg.PS.Z = !m;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void LDX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  t = em->m_bus->Read( em, a );

	em->m_reg.X    =  t;
	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void LDY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  t = em->m_bus->Read( em, a );

	em->m_reg.Y    =  t;
	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void LSR( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );

	em->m_reg.PS.C =  m  & 0x01;
	em->m_reg.PS.N =  0x00;
	m              =  m >> 0x01;
	em->m_reg.PS.Z = !m;

	em->m_bus->Write( em, a, m );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void LSR_A( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.N =  0x00;
	em->m_reg.PS.C =  em->m_reg.A  & 0x01;
	em->m_reg.A    =  em->m_reg.A >> 0x01;
	em->m_reg.PS.Z = !em->m_reg.A;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void NOP( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void ORA( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );

	em->m_reg.A    =  em->m_reg.A | m;
	em->m_reg.PS.N =  em->m_reg.A >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.A;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void PHA( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.A );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void PHP( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.PS.m_data );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void PLA( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{	
	em->m_reg.A    =  em->m_bus->Read( em, ++em->m_reg.SP | 0x100 );
	em->m_reg.PS.N =  em->m_reg.A >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.A;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void PLP( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.m_data = em->m_bus->Read( em, ++em->m_reg.SP | 0x100 );
	em->m_reg.PS.B      = 0;
	em->m_reg.PS.P      = 1;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void ROL( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );

	u16 t = ((u16)m << 0x01) | em->m_reg.PS.C;

	em->m_reg.PS.C =  t >> 0x08;
	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;
	em->m_bus->Write( em, a, t );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void ROL_A( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{	
	u16 t  = ((u16)em->m_reg.A << 0x01) | em->m_reg.PS.C;
	em->m_reg.PS.C =  t >> 0x08;
	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;
	em->m_reg.A    =  t;
	
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void ROR( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{	
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a );
	u16 t = m | ((u16)em->m_reg.PS.C << 8);
	em->m_reg.PS.C =  t  & 0x01;
	t              =  t >> 0x01;
	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;

	em->m_bus->Write( em, a, t );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void ROR_A( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{	
	u16 t  = em->m_reg.A | ((u16)em->m_reg.PS.C << 8);
	
	em->m_reg.PS.C =  t  & 0x01;
	t              =  t >> 0x01;
	em->m_reg.PS.N =  t >> 0x07;
	em->m_reg.PS.Z = !t;
	em->m_reg.A    =  t;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void RTI( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{	
	em->m_reg.PS.m_data = em->m_bus->Read( em, ++em->m_reg.SP | 0x100 );
	em->m_reg.PS.P      = 1;

	em->m_reg.PC = em->m_bus->Read( em, ++em->m_reg.SP | 0x100 ) |  \
				 ( em->m_bus->Read( em, ++em->m_reg.SP | 0x100 ) << 8 );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void RTS( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{	
	struct MOS6502_BUS* b  = em->m_bus;

	em->m_reg.PC = em->m_bus->Read( em, ++em->m_reg.SP | 0x100 ) | \
				 ( em->m_bus->Read( em, ++em->m_reg.SP | 0x100 ) << 8 );

	em->m_reg.PC++;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void SBC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	u8  m = em->m_bus->Read( em, a ) ^ 0xFF;
	u16 t = em->m_reg.A + m + em->m_reg.PS.C;

	em->m_reg.PS.N =  (t >> 0x07);
	em->m_reg.PS.Z = !(t &  0xFF);
	em->m_reg.PS.C =  (t >> 0x08);
	em->m_reg.PS.V =  (~(em->m_reg.A ^ m) & 
						(em->m_reg.A ^ t) & 0x80) >> 0x07;

	em->m_reg.A    =  (t &  0xFF);

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void SEC( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.C = 1;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void SED( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.D = 1;
	
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void SEI( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.PS.I = 1;
	
	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void STA( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	em->m_bus->Write( em, a, em->m_reg.A );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void STX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	em->m_bus->Write( em, a, em->m_reg.X );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void STY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	u16 a = this_ptr->Mode( this_ptr, em );
	em->m_bus->Write( em, a, em->m_reg.Y );

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void TAX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.X    =  em->m_reg.A;
	em->m_reg.PS.N =  em->m_reg.X >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.X;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}		

FUNC_OPT
static void TAY( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.Y    =  em->m_reg.A;
	em->m_reg.PS.N =  em->m_reg.Y >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.Y;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void TSX( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.X    =  em->m_reg.SP;
	em->m_reg.PS.N =  em->m_reg.X >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.X;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void TXA( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.A    =  em->m_reg.X;
	em->m_reg.PS.N =  em->m_reg.A >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.A;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void TXS( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.SP =  em->m_reg.X;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

FUNC_OPT
static void TYA( struct MOS6502_INS* this_ptr, struct MOS6502* em )
{
	em->m_reg.A    =  em->m_reg.Y;
	em->m_reg.PS.N =  em->m_reg.A >> 0x07;
	em->m_reg.PS.Z = !em->m_reg.A;

	for( u8 iter = 0; iter < this_ptr->m_clock; iter++ )
		em->m_bus->Tick( em );
}

/*=================================================================
	Interrupts()
===================================================================
	*
*/
void IRQ( struct MOS6502* em )
{
	if( em->m_reg.PS.I != 1 )
	{
		em->m_reg.PS.B = 0;
		em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.PC >> 8 );
		em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.PC );
		em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.PS.m_data );
		em->m_reg.PS.I = 0;
		em->m_reg.PC   = em->m_bus->Read(em, IRQ_ADDRESS) | (em->m_bus->Read(em, IRQ_ADDRESS + 1) << 8);

		/* IRQ Takes 7 clock cycles */
		for( u8 iter = 0; iter < 7; iter++ )
			em->m_bus->Tick( em );
	}
}

void NMI( struct MOS6502* em )
{
	em->m_reg.PS.B = 0;

	em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.PC >> 8 );
	em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.PC );
	em->m_bus->Write( em, em->m_reg.SP-- | 0x100, em->m_reg.PS.m_data );

	em->m_reg.PS.I      = 1;
	em->m_reg.PC        = em->m_bus->Read(em, NMI_ADDRESS) | (em->m_bus->Read(em, NMI_ADDRESS + 1) << 8);

	/* IRQ Takes 7 clock cycles */
	for( u8 iter = 0; iter < 7; iter++ )
			em->m_bus->Tick( em );
}

void RST( struct MOS6502* em )
{
	em->m_reg.A         = 0;
	em->m_reg.X         = 0;
	em->m_reg.Y         = 0;
	em->m_reg.PC        = em->m_bus->Read(em, RST_ADDRESS) | (em->m_bus->Read(em, RST_ADDRESS + 1) << 8);
	em->m_reg.SP        = 0xFD;
	em->m_reg.PS.m_data = 0;	
	
	/* IRQ Takes 7 clock cycles */
	for( u8 iter = 0; iter < 9; iter++ )
		em->m_bus->Tick( em );
}

struct MOS6502_INS g_instructionSet[ 256 ] =
{
	{   BRK,      IMPLIED, "BRK", 7, false  }, // 0x00
	{   ORA,    INDIRECTX, "ORA", 6, false  }, // 0x01
	{},  // 0x02
	{},  // 0x03
	{},  // 0x04
	{   ORA,     ZEROPAGE, "ORA", 3, false  }, // 0x05
	{   ASL,     ZEROPAGE, "ASL", 5, false  }, // 0x06
	{},  // 0x07
	{   PHP,      IMPLIED, "PHP", 3, false  }, // 0x08
	{   ORA,    IMMEDIATE, "ORA", 2, false  }, // 0x09
	{ ASL_A,  ACCUMULATOR, "ASL", 2, false  }, // 0x0A
	{},  // 0x0B
	{},  // 0x0C
	{   ORA,     ABSOLUTE, "ORA", 4, false  }, // 0x0D
	{   ASL,     ABSOLUTE, "ASL", 7, false  }, // 0x0E
	{},  // 0x0F
	{   BPL,     RELATIVE, "BPL", 2,  true  }, // 0x10
	{   ORA,    INDIRECTY, "ORA", 5, false  }, // 0x11
	{},  // 0x12
	{},  // 0x13
	{},  // 0x14
	{   ORA,    ZEROPAGEX, "ORA", 4, false  }, // 0x15
	{   ASL,    ZEROPAGEX, "ASL", 6, false  }, // 0x16
	{},  // 0x17
	{   CLC,      IMPLIED, "CLC", 2, false  }, // 0x18
	{   ORA,    ABSOLUTEY, "ORA", 4,  true  }, // 0x19
	{},  // 0x1A
	{},  // 0x1B
	{},  // 0x1C
	{   ORA,    ABSOLUTEX, "ORA", 4,  true  }, // 0x1D
	{   ASL,    ABSOLUTEX, "ASL", 7, false  }, // 0x1E
	{},  // 0x1F
	{   JSR,     ABSOLUTE, "JSR", 6, false  }, // 0x20
	{   AND,    INDIRECTX, "AND", 6, false  }, // 0x21
	{},  // 0x22
	{},  // 0x23
	{   BIT,     ZEROPAGE, "BIT", 3, false  }, // 0x24
	{   AND,     ZEROPAGE, "AND", 3, false  }, // 0x25
	{   ROL,     ZEROPAGE, "ROL", 5, false  }, // 0x26
	{},  // 0x27
	{   PLP,      IMPLIED, "PLP", 3, false  }, // 0x28
	{   AND,    IMMEDIATE, "AND", 2, false  }, // 0x29
	{ ROL_A,  ACCUMULATOR, "ROL", 2, false  }, // 0x2A
	{},  // 0x2B
	{   BIT,     ABSOLUTE, "BIT", 4, false  }, // 0x2C
	{   AND,     ABSOLUTE, "AND", 4, false  }, // 0x2D
	{   ROL,     ABSOLUTE, "ROL", 6, false  }, // 0x2E
	{},  // 0x2F
	{   BMI,     RELATIVE, "BMI", 2,  true  }, // 0x30
	{   AND,    INDIRECTY, "AND", 5, false  }, // 0x31
	{},  // 0x32
	{},  // 0x33
	{},  // 0x34
	{   AND,    ZEROPAGEX, "AND", 4, false  }, // 0x35
	{   ROL,    ZEROPAGEX, "ROL", 6, false  }, // 0x36
	{},  // 0x37
	{   SEC,      IMPLIED, "SEC", 2, false  }, // 0x38
	{   AND,    ABSOLUTEY, "AND", 4,  true  }, // 0x39
	{},  // 0x3A
	{},  // 0x3B
	{},  // 0x3C
	{   AND,    ABSOLUTEX, "AND", 4,  true  }, // 0x3D
	{   ROL,    ABSOLUTEX, "ROL", 7, false  }, // 0x3E
	{},  // 0x3F
	{   RTI,      IMPLIED, "RTI", 6, false  }, // 0x40
	{   EOR,    INDIRECTX, "EOR", 2, false  }, // 0x41
	{},  // 0x42
	{},  // 0x43
	{},  // 0x44
	{   EOR,     ZEROPAGE, "EOR", 2, false  }, // 0x45
	{   LSR,     ZEROPAGE, "LSR", 5, false  }, // 0x46
	{},  // 0x47
	{   PHA,      IMPLIED, "PHA", 3, false  }, // 0x48
	{   EOR,    IMMEDIATE, "EOR", 2, false  }, // 0x49
	{ LSR_A,  ACCUMULATOR, "LSR", 2, false  }, // 0x4A
	{},  // 0x4B
	{   JMP,     ABSOLUTE, "JMP", 3, false  }, // 0x4C
	{   EOR,     ABSOLUTE, "EOR", 2, false  }, // 0x4D
	{   LSR,     ABSOLUTE, "LSR", 6, false  }, // 0x4E
	{},  // 0x4F
	{   BVC,     RELATIVE, "BVC", 2,  true  }, // 0x50
	{   EOR,    INDIRECTY, "EOR", 2, false  }, // 0x51
	{},  // 0x52
	{},  // 0x53
	{},  // 0x54
	{   EOR,    ZEROPAGEX, "EOR", 2, false  }, // 0x55
	{   LSR,    ZEROPAGEX, "LSR", 6, false  }, // 0x56
	{},  // 0x57
	{   CLI,      IMPLIED, "CLI", 2, false  }, // 0x58
	{   EOR,    ABSOLUTEY, "EOR", 2, false  }, // 0x59
	{},  // 0x5A
	{},  // 0x5B
	{},  // 0x5C
	{   EOR,    ABSOLUTEX, "EOR", 2, false  }, // 0x5D
	{   LSR,    ABSOLUTEX, "LSR", 7, false  }, // 0x5E
	{},  // 0x5F
	{   RTS,      IMPLIED, "RTS", 6, false  }, // 0x60
	{   ADC,    INDIRECTX, "ADC", 6, false  }, // 0x61
	{},  // 0x62
	{},  // 0x63
	{},  // 0x64
	{   ADC,     ZEROPAGE, "ADC", 3, false  }, // 0x65
	{   ROR,     ZEROPAGE, "ROR", 5, false  }, // 0x66
	{},  // 0x67
	{   PLA,      IMPLIED, "PLA", 3, false  }, // 0x68
	{   ADC,    IMMEDIATE, "ADC", 2, false  }, // 0x69
	{ ROR_A,  ACCUMULATOR, "ROR", 2, false  }, // 0x6A
	{},  // 0x6B
	{   JMP,     INDIRECT, "JMP", 5, false  }, // 0x6C
	{   ADC,     ABSOLUTE, "ADC", 4, false  }, // 0x6D
	{   ROR,     ABSOLUTE, "ROR", 6, false  }, // 0x6E
	{},  // 0x6F
	{   BVS,     RELATIVE, "BVS", 2,  true  }, // 0x70
	{   ADC,    INDIRECTY, "ADC", 5,  true  }, // 0x71
	{},  // 0x72
	{},  // 0x73
	{},  // 0x74
	{   ADC,    ZEROPAGEX, "ADC", 4, false  }, // 0x75
	{   ROR,    ZEROPAGEX, "ROR", 6, false  }, // 0x76
	{},  // 0x77
	{   SEI,      IMPLIED, "SEI", 2, false  }, // 0x78
	{   ADC,    ABSOLUTEY, "ADC", 4,  true  }, // 0x79
	{},  // 0x7A
	{},  // 0x7B
	{},  // 0x7C
	{   ADC,    ABSOLUTEX, "ADC", 4,  true  }, // 0x7D
	{   ROR,    ABSOLUTEX, "ROR", 7, false  }, // 0x7E
	{},  // 0x7F
	{},  // 0x80
	{   STA,    INDIRECTX, "STA", 6, false  }, // 0x81
	{},  // 0x82
	{},  // 0x83
	{   STY,     ZEROPAGE, "STY", 3, false  }, // 0x84
	{   STA,     ZEROPAGE, "STA", 3, false  }, // 0x85
	{   STX,     ZEROPAGE, "STX", 3, false  }, // 0x86
	{},  // 0x87
	{   DEY,      IMPLIED, "DEY", 2, false  }, // 0x88
	{},  // 0x89
	{   TXA,      IMPLIED, "TXA", 2, false  }, // 0x8A
	{},  // 0x8B
	{   STY,     ABSOLUTE, "STY", 4, false  }, // 0x8C
	{   STA,     ABSOLUTE, "STA", 4, false  }, // 0x8D
	{   STX,     ABSOLUTE, "STX", 4, false  }, // 0x8E
	{},  // 0x8F
	{   BCC,     RELATIVE, "BCC", 2,  true  }, // 0x90
	{   STA,    INDIRECTY, "STA", 6, false  }, // 0x91
	{},  // 0x92
	{},  // 0x93
	{   STY,    ZEROPAGEX, "STY", 4, false  }, // 0x94
	{   STA,    ZEROPAGEX, "STA", 4, false  }, // 0x95
	{   STX,    ZEROPAGEY, "STX", 4, false  }, // 0x96
	{},  // 0x97
	{   TYA,      IMPLIED, "TYA", 2, false  }, // 0x98
	{   STA,    ABSOLUTEY, "STA", 5, false  }, // 0x99
	{   TXS,      IMPLIED, "TXS", 2, false  }, // 0x9A
	{},  // 0x9B
	{},  // 0x9C
	{   STA,    ABSOLUTEX, "STA", 5, false  }, // 0x9D
	{},  // 0x9E
	{},  // 0x9F
	{   LDY,    IMMEDIATE, "LDY", 2, false  }, // 0xA0
	{   LDA,    INDIRECTX, "LDA", 6, false  }, // 0xA1
	{   LDX,    IMMEDIATE, "LDX", 2, false  }, // 0xA2
	{},  // 0xA3
	{   LDY,     ZEROPAGE, "LDY", 3, false  }, // 0xA4
	{   LDA,     ZEROPAGE, "LDA", 3, false  }, // 0xA5
	{   LDX,     ZEROPAGE, "LDX", 3, false  }, // 0xA6
	{},  // 0xA7
	{   TAY,      IMPLIED, "TAY", 2, false  }, // 0xA8
	{   LDA,    IMMEDIATE, "LDA", 2, false  }, // 0xA9
	{   TAX,      IMPLIED, "TAX", 2, false  }, // 0xAA
	{},  // 0xAB
	{   LDY,     ABSOLUTE, "LDY", 4, false  }, // 0xAC
	{   LDA,     ABSOLUTE, "LDA", 4, false  }, // 0xAD
	{   LDX,     ABSOLUTE, "LDX", 4, false  }, // 0xAE
	{},  // 0xAF
	{   BCS,     RELATIVE, "BCS", 2,  true  }, // 0xB0
	{   LDA,    INDIRECTY, "LDA", 5,  true  }, // 0xB1
	{},  // 0xB2
	{},  // 0xB3
	{   LDY,    ZEROPAGEX, "LDY", 4, false  }, // 0xB4
	{   LDA,    ZEROPAGEX, "LDA", 4, false  }, // 0xB5
	{   LDX,    ZEROPAGEY, "LDX", 4, false  }, // 0xB6
	{},  // 0xB7
	{   CLV,      IMPLIED, "CLV", 2, false  }, // 0xB8
	{   LDA,    ABSOLUTEY, "LDA", 4,  true  }, // 0xB9
	{   TSX,      IMPLIED, "TSX", 2, false  }, // 0xBA
	{},  // 0xBB
	{   LDY,    ABSOLUTEX, "LDY", 4,  true  }, // 0xBC
	{   LDA,    ABSOLUTEX, "LDA", 4,  true  }, // 0xBD
	{   LDX,    ABSOLUTEY, "LDX", 4,  true  }, // 0xBE
	{},  // 0xBF
	{   CPY,    IMMEDIATE, "CPY", 2, false  }, // 0xC0
	{   CMP,    INDIRECTX, "CMP", 6, false  }, // 0xC1
	{},  // 0xC2
	{},  // 0xC3
	{   CPY,     ZEROPAGE, "CPY", 3, false  }, // 0xC4
	{   CMP,     ZEROPAGE, "CMP", 3, false  }, // 0xC5
	{   DEC,     ZEROPAGE, "DEC", 5, false  }, // 0xC6
	{},  // 0xC7
	{   INY,      IMPLIED, "INY", 2, false  }, // 0xC8
	{   CMP,    IMMEDIATE, "CMP", 2, false  }, // 0xC9
	{   DEX,      IMPLIED, "DEX", 2, false  }, // 0xCA
	{},  // 0xCB
	{   CPY,     ABSOLUTE, "CPY", 4, false  }, // 0xCC
	{   CMP,     ABSOLUTE, "CMP", 4, false  }, // 0xCD
	{   DEC,     ABSOLUTE, "DEC", 6, false  }, // 0xCE
	{},  // 0xCF
	{   BNE,     RELATIVE, "BNE", 2,  true  }, // 0xD0
	{   CMP,    INDIRECTY, "CMP", 5,  true  }, // 0xD1
	{},  // 0xD2
	{},  // 0xD3
	{},  // 0xD4
	{   CMP,    ZEROPAGEX, "CMP", 4, false  }, // 0xD5
	{   DEC,    ZEROPAGEX, "DEC", 6, false  }, // 0xD6
	{},  // 0xD7
	{   CLD,      IMPLIED, "CLD", 2, false  }, // 0xD8
	{   CMP,    ABSOLUTEY, "CMP", 4,  true  }, // 0xD9
	{},  // 0xDA
	{},  // 0xDB
	{},  // 0xDC
	{   CMP,    ABSOLUTEX, "CMP", 4,  true  }, // 0xDD
	{   DEC,    ABSOLUTEX, "DEC", 7, false  }, // 0xDE
	{},  // 0xDF
	{   CPX,    IMMEDIATE, "CPX", 2, false  }, // 0xE0
	{   SBC,    INDIRECTX, "SBC", 6, false  }, // 0xE1
	{},  // 0xE2
	{},  // 0xE3
	{   CPX,     ZEROPAGE, "CPX", 3, false  }, // 0xE4
	{   SBC,     ZEROPAGE, "SBC", 3, false  }, // 0xE5
	{   INC,     ZEROPAGE, "INC", 2, false  }, // 0xE6
	{},  // 0xE7
	{   INX,      IMPLIED, "INX", 2, false  }, // 0xE8
	{   SBC,    IMMEDIATE, "SBC", 2, false  }, // 0xE9
	{   NOP,      IMPLIED, "NOP", 2, false  }, // 0xEA
	{},  // 0xEB
	{   CPX,     ABSOLUTE, "CPX", 4, false  }, // 0xEC
	{   SBC,     ABSOLUTE, "SBC", 4, false  }, // 0xED
	{   INC,     ABSOLUTE, "INC", 2, false  }, // 0xEE
	{},  // 0xEF
	{   BEQ,     RELATIVE, "BEQ", 2,  true  }, // 0xF0
	{   SBC,    INDIRECTY, "SBC", 5, false  }, // 0xF1
	{},  // 0xF2
	{},  // 0xF3
	{},  // 0xF4
	{   SBC,    ZEROPAGEX, "SBC", 4, false  }, // 0xF5
	{   INC,    ZEROPAGEX, "INC", 2, false  }, // 0xF6
	{},  // 0xF7
	{   SED,      IMPLIED, "SED", 2, false  }, // 0xF8
	{   SBC,    ABSOLUTEY, "SBC", 4,  true  }, // 0xF9
	{},  // 0xFA
	{},  // 0xFB
	{},  // 0xFC
	{   SBC,    ABSOLUTEX, "SBC", 4,  true  }, // 0xFD
	{   INC,    ABSOLUTEX, "INC", 2, false  }, // 0xFE
	{},  // 0xFF
};