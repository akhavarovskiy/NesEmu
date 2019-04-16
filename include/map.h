#ifndef NES_MAP_H__
#define NES_MAP_H__

#include <6502.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NES_MAP
{
	uint8_t (*Read )( struct MOS6502* em, uint16_t addr );
	void    (*Write)( struct MOS6502* em, uint16_t addr, uint8_t v );
	
	void* mapper_data;
	struct NES_ROM* m_rom;
	uint8_t         m_id;
	uint8_t*        m_selectedPROM[2];
	uint8_t*        m_selectedCROM;
}
NES_MAP;

/*==================================================================================
	CreateMapper() - Will partition ROM data, based on the design of the cartridge.
====================================================================================
	em -> Pointer to the emulated machine object
	mapperID -> The mapper id used by the game.
	return -> *Will return NULL on an unsupported mapper ID.
*/
NES_MAP* NesEx_CreateMAP( struct NES_ROM* rom );

#ifdef __cplusplus
}
#endif

#endif /* NES_MAP_H__ */