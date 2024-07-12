#include "LibraryHandler.h"

#include <SDL.h>
#include <SDL_image.h>    
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>

bool LibraryHandler::Create(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to initialize SDL! " << SDL_GetError();
	#endif

		return false;
	}

	if (!(IMG_Init(IMG_InitFlags::IMG_INIT_PNG) & IMG_InitFlags::IMG_INIT_PNG))
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to initialize SDL_Image! " << IMG_GetError();
	#endif

		return false;
	}

	if (TTF_Init() == -1)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to initialize SDL_TTF! " << TTF_GetError();
	#endif

		return false;
	}

	if (!(Mix_Init(MIX_InitFlags::MIX_INIT_MP3 | MIX_InitFlags::MIX_INIT_OGG) & (MIX_InitFlags::MIX_INIT_MP3 | MIX_InitFlags::MIX_INIT_OGG)))
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to initialize SDL_Mixer! " << Mix_GetError();
	#endif

		return false;
	}

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to open audio device! " << Mix_GetError();
	#endif

		return false;
	}

	// If you have some other external library/libraries that you need to create/start up,
	// you can do it here

	return true;
}

void LibraryHandler::Destroy(void)
{
	// If you have some other external library/libraries that you have created/started up in the
	// 'Create' function above, you should destroy/shut it down here

	Mix_Quit();	
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}