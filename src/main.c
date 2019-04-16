#include "..//include//main.h"
#include "..//include//jpd.h"

#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

#include <sys/time.h>
#include <sys/resource.h>


#define NES_NTSC_CLOCK_SPEED_MHZ               1.789773       /* MHZ */
#define NES_NTSC_CLOCK_SPEED_HZ                1789773.0      /*  HZ */
#define NES_PAL_CLOCK_SPEED_MHZ                1.662607       /* MHZ */
#define NES_PAL_CLOCK_SPEED_HZ                 1662607.0      /*  HZ */

#define MOS6502_CLOCK_SPEED_HZ                 NES_NTSC_CLOCK_SPEED_HZ                 /* HZ */
#define MOS6502_SECONDS_PER_CLOCK_CYCLE        1.0/MOS6502_CLOCK_SPEED_HZ              /* SEC */
#define MOS6502_MILLISECONDS_PER_CLOCK_CYCLE   MOS6502_SECONDS_PER_CLOCK_CYCLE*1000.0  /* MIL SEC */
#define MOS6502_MICROSECONDS_PER_CLOCK_CYCLE   MOS6502_MILLISECONDS_PER_CLOCK_CYCLE*1000.0  /* MIL SEC */


int main( int argc, char* argv[] )
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		fprintf( stderr, "[ Error ] ApplicationWindow()::SDL_Init(), : %s\n", SDL_GetError() );
		exit(1);
	}

	SDL_Window* wnd = \
	SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256*3, 240*3, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN );
	if( wnd == NULL )
	{
		fprintf( stderr, "[ Error ] ApplicationWindow()::SDL_CreateWindow(), %s \n", SDL_GetError() );
		exit(1);
	}
	
	SDL_SetWindowMinimumSize( wnd, 256*3, 240*3 );
	SDL_SetWindowPosition   ( wnd, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	SDL_Renderer* renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED );
	if( renderer == NULL ) {
		fprintf( stderr, "[ Error ] SDL_CreateRenderer(), %s\n", SDL_GetError() );
		SDL_Quit();
	}
	if( SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2" ) < 0 ) {
		fprintf( stderr, "[ Error ] SDL_SetHint(), %s\n", SDL_GetError() );
		SDL_Quit();
	}

	if( SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1" ) < 0 ) {
		fprintf( stderr, "[ Error ] SDL_SetHint(), %s\n", SDL_GetError() );
		SDL_Quit();
	}
	
	if( SDL_RenderSetLogicalSize(renderer,  256, 240) < 0 ) {
		fprintf( stderr, "[ Error ] SDL_RenderSetLogicalSize(), %s\n", SDL_GetError() );
		SDL_Quit();
	}
	
	SDL_Texture* text = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 256, 240 ); 
	if( text == NULL )  {
		fprintf( stderr, "[ Error ] SDL_CreateTexture(), %s\n", SDL_GetError() );
		SDL_Quit();
	}
	SDL_SetRenderTarget   ( renderer, text );
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 ); 

	fprintf( stdout, "argv[1] : %s\n", argv[1] );

	NES_ROM* rom = NesEx_CreateROM( argv[1] );

	NES_PPU* ppu = NesEx_CreatePPU();

	NES_BUS* bus = NesEx_CreateBUS( rom, ppu, NULL );

	MOS6502*  em = CreateMachine( (MOS6502_BUS*)bus );
	
	u32 MOSClock          = 0;
	u32 MOSClockLastFrame = 0;

	u32 CPUClock          = 0;
	u32 CPUClockLastFrame = (clock());

	for(MOS6502_INS* i = em->Run( em ); i != NULL; i = em->Run( em ) )
	{
		if( ppu->m_frameReady )
		{
			SDL_UpdateTexture( text, NULL, ppu->m_frameBuffer, 256*4 );
			SDL_RenderClear  ( renderer );
			SDL_RenderCopy   ( renderer, text, NULL, NULL );
			SDL_RenderPresent( renderer );

			SDL_Event lvEvent;
			while( SDL_PollEvent( &lvEvent ) )
			{
				switch( lvEvent.type )
				{
				case SDL_QUIT:
					SDL_Quit();
					exit(0);
					break;
				default:
					HandleButton( lvEvent );
					break;
				}
			}
			MOSClock = em->m_bus->m_ticks;
			CPUClock = clock();

			if( MOSClockLastFrame <= 0) 
				MOSClock = 0;

			if( CPUClockLastFrame <= 0 )
				CPUClock = 0;
			
			int32_t delta = (int)(MOSClock - MOSClockLastFrame)*MOS6502_MICROSECONDS_PER_CLOCK_CYCLE - //
					    	(int)(CPUClock - CPUClockLastFrame);

			if( delta > 0 )
				usleep(delta);
			else 
				fprintf( stdout, "[ Info ] Delta %d\n", delta );

			MOSClockLastFrame = em->m_bus->m_ticks;
			CPUClockLastFrame = clock();	

			ppu->m_frameReady = false;
		}
	}
	
	return 0;
}
