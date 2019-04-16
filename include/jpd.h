#ifndef NES_JPD_H__
#define NES_JPD_H__

#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

typedef uint8_t u8;


u8 ReadState( u8 val );

void WriteStrobe( bool val );

void HandleButton( SDL_Event event );

#endif