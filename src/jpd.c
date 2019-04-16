#include "..//include//jpd.h"

enum CKS 
{
	KS_UP = 0,
	KS_DOWN
};

enum CKI 
{
	KI_UP = 0,
	KI_DOWN,
	KI_LEFT,
	KI_RIGHT,
	KI_SELECT,
	KI_START,
	KI_A,
	KI_B
};

/* Internal Latch */
bool STROBE_LATCH;

/* Registers */
u8 REGISTER[ 2 ] = { 0 };



u8 CONTROLLER_KEY_STATE[2][8];

u8 GetJoyPadState( u8 val )
{
	if( val > 2 )
		return 0;

	u8 ret = 0;
	ret |= CONTROLLER_KEY_STATE[val][KI_A]      << 0;
	ret |= CONTROLLER_KEY_STATE[val][KI_B]      << 1;
	ret |= CONTROLLER_KEY_STATE[val][KI_SELECT] << 2;
	ret |= CONTROLLER_KEY_STATE[val][KI_START]  << 3;
	ret |= CONTROLLER_KEY_STATE[val][KI_UP]     << 4;
	ret |= CONTROLLER_KEY_STATE[val][KI_DOWN]   << 5; 
	ret |= CONTROLLER_KEY_STATE[val][KI_LEFT]   << 6; 
	ret |= CONTROLLER_KEY_STATE[val][KI_RIGHT]  << 7; 

	return ret;    
}
/**/

u8 ReadState( u8 val )
{
	if( STROBE_LATCH )
		return 0x40 | (GetJoyPadState( val ) & 1);
	
	u8 ret = 0x40 | (REGISTER[ val ] & 1);
	REGISTER[ val ] = 0x80 | (REGISTER[ val ] >> 1 );
	return ret;
}

void WriteStrobe( bool val )
{
	if( STROBE_LATCH && !val ) {
		for( int i = 0; i < 1; i++ )
			REGISTER[ i ] = GetJoyPadState( i );
	}
	STROBE_LATCH = val;
}


void HandleButton( SDL_Event event )
{            
	signed int m = 0;
	switch ( event.type )
	{   
	case SDL_KEYUP:      
		switch( event.key.keysym.sym )
		{
		case SDLK_a:
				CONTROLLER_KEY_STATE[0][KI_LEFT] = KS_UP;
				break;
		case SDLK_w:
				CONTROLLER_KEY_STATE[0][KI_UP] = KS_UP;
				break;
		case SDLK_s:
				CONTROLLER_KEY_STATE[0][KI_DOWN] = KS_UP;
				break;
		case SDLK_d:
				CONTROLLER_KEY_STATE[0][KI_RIGHT] = KS_UP;
				break;
		case SDLK_k:
				CONTROLLER_KEY_STATE[0][KI_B] = KS_UP;
				break;
		case SDLK_l:
				CONTROLLER_KEY_STATE[0][KI_A] = KS_UP;
				break;
		case SDLK_RETURN:
				CONTROLLER_KEY_STATE[0][KI_START] = KS_UP;
				break;
		case SDLK_RSHIFT:
				CONTROLLER_KEY_STATE[0][KI_SELECT] = KS_UP;
				break;
		}
		break;

	case SDL_KEYDOWN:
		switch( event.key.keysym.sym )
		{
		case SDLK_a:
				CONTROLLER_KEY_STATE[0][KI_LEFT]   = KS_DOWN;
				break;                        
		case SDLK_w:
				CONTROLLER_KEY_STATE[0][KI_UP]     = KS_DOWN;
				break;
		case SDLK_s:
				CONTROLLER_KEY_STATE[0][KI_DOWN]   = KS_DOWN;
				break;
		case SDLK_d:
				CONTROLLER_KEY_STATE[0][KI_RIGHT]  = KS_DOWN;
				break;
		case SDLK_k:
				CONTROLLER_KEY_STATE[0][KI_B]      = KS_DOWN;
				break;
		case SDLK_l:
				CONTROLLER_KEY_STATE[0][KI_A]      = KS_DOWN;
				break;
		case SDLK_RETURN:
				CONTROLLER_KEY_STATE[0][KI_START]  = KS_DOWN;
				break;
		case SDLK_RSHIFT:
				CONTROLLER_KEY_STATE[0][KI_SELECT] = KS_DOWN;                
				break;
		}
		break;
	/*
	case SDL_CONTROLLERAXISMOTION:
		if( SDL_GameControllerGetAxis(g_SDLGameController, SDL_CONTROLLER_AXIS_LEFTX ) > 25000 ) {
			CONTROLLER_KEY_STATE[0][KI_RIGHT] = KS_DOWN;                                                         
		} else {
			CONTROLLER_KEY_STATE[0][KI_RIGHT] = KS_UP;                                                                             
		}
		if( SDL_GameControllerGetAxis(g_SDLGameController, SDL_CONTROLLER_AXIS_LEFTX ) < -25000 ) {
			CONTROLLER_KEY_STATE[0][KI_LEFT] = KS_DOWN;                                                         
		} else {
			CONTROLLER_KEY_STATE[0][KI_LEFT] = KS_UP;                                                                             
		}
		if( SDL_GameControllerGetAxis(g_SDLGameController, SDL_CONTROLLER_AXIS_LEFTY ) > 25000 ) {
			CONTROLLER_KEY_STATE[0][KI_DOWN] = KS_DOWN;                                                         
		} else {
			CONTROLLER_KEY_STATE[0][KI_DOWN] = KS_UP;                                                                             
		}                
		if( SDL_GameControllerGetAxis(g_SDLGameController, SDL_CONTROLLER_AXIS_LEFTY ) < -25000 ) {
			CONTROLLER_KEY_STATE[0][KI_UP] = KS_DOWN;                                                         
		} else {
			CONTROLLER_KEY_STATE[0][KI_UP] = KS_UP;                                                                                                 
		}
		break;*/

	case SDL_CONTROLLERBUTTONDOWN:
		switch( event.cbutton.button )
		{
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			CONTROLLER_KEY_STATE[0][KI_UP] = KS_DOWN;
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			CONTROLLER_KEY_STATE[0][KI_DOWN] = KS_DOWN;                               
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			CONTROLLER_KEY_STATE[0][KI_LEFT] = KS_DOWN;                                                                                                                
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			CONTROLLER_KEY_STATE[0][KI_RIGHT] = KS_DOWN;                                                         
			break;

		case SDL_CONTROLLER_BUTTON_START:
			CONTROLLER_KEY_STATE[0][KI_START] = KS_DOWN;                                                       
			break;
		
		case SDL_CONTROLLER_BUTTON_BACK:
			CONTROLLER_KEY_STATE[0][KI_SELECT] = KS_DOWN;                                                        
			break;

		case SDL_CONTROLLER_BUTTON_A:
			CONTROLLER_KEY_STATE[0][KI_B] = KS_DOWN;                                                                         
			break;

		case SDL_CONTROLLER_BUTTON_B:
			CONTROLLER_KEY_STATE[0][KI_A] = KS_DOWN;                                                                                          
			break; 

		case SDL_CONTROLLER_BUTTON_GUIDE:                
			break;
		};
		break;

	case SDL_CONTROLLERBUTTONUP:
		switch( event.cbutton.button )
		{
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			CONTROLLER_KEY_STATE[0][KI_UP] = KS_UP;
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			CONTROLLER_KEY_STATE[0][KI_DOWN] = KS_UP;                               
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			CONTROLLER_KEY_STATE[0][KI_LEFT] = KS_UP;                                                                                                                
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			CONTROLLER_KEY_STATE[0][KI_RIGHT] = KS_UP;                                                         
			break;

		case SDL_CONTROLLER_BUTTON_START:
			CONTROLLER_KEY_STATE[0][KI_START] = KS_UP;                                                         
			break;
		
		case SDL_CONTROLLER_BUTTON_BACK:
			CONTROLLER_KEY_STATE[0][KI_SELECT] = KS_UP;                                                         
			break;

		case SDL_CONTROLLER_BUTTON_A:
			CONTROLLER_KEY_STATE[0][KI_B] = KS_UP;                                                         
			break;

		case SDL_CONTROLLER_BUTTON_B:
			CONTROLLER_KEY_STATE[0][KI_A] = KS_UP;                                                                     
			break; 
		};
		break;
	}
}