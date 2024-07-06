#pragma once 

#include <SDL_mixer.h>
#include <string>

class AudioHandler 
{
public: 

	 AudioHandler(void) {}
	~AudioHandler(void) {}

	Mix_Chunk*	CreateSound(const std::string& fileName);
	Mix_Music*	CreateMusic(const std::string& fileName);

	void		DestroySound(Mix_Chunk* chunk);
	void		DestroyMusic(Mix_Music* music);

};