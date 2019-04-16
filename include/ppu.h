#ifndef NES_PPU_H__
#define NES_PPU_H__

#include <6502.h>
#include <stddef.h>
#include <stdint.h>

// Integer types
typedef uint_least32_t u32;
typedef uint_least16_t u16;
typedef uint_least8_t   u8;
typedef  int_least8_t   s8;

typedef struct NES_PPU
{
	void (*Tick )( struct MOS6502* em );
	u8   (*Read )( struct MOS6502* em, u16 addr );
	void (*Write)( struct MOS6502* em, u16 addr, u8 v );

	uint32_t* m_frameBuffer;
	bool      m_frameReady;
}
NES_PPU;


NES_PPU* NesEx_CreatePPU( void );

#endif