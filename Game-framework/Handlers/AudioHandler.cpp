#include "Pch.h"
#include "AudioHandler.h"

Mix_Chunk* CAudioHandler::CreateSound(const std::string& fileName)
{
	Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());

	if (!chunk)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to load sound '" << fileName.c_str() << "'" << std::endl;
		std::cout << Mix_GetError() << std::endl;
	#endif
	}

	return chunk;
}

Mix_Music* CAudioHandler::CreateMusic(const std::string& fileName)
{
	Mix_Music* music = Mix_LoadMUS(fileName.c_str());

	if (!music)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to load music '" << fileName.c_str() << "'" << std::endl;
		std::cout << Mix_GetError() << std::endl;
	#endif
	}

	return music;
}

void CAudioHandler::DestroySound(Mix_Chunk* chunk)
{
	if (!chunk)
		return;

	Mix_FreeChunk(chunk);
}

void CAudioHandler::DestroyMusic(Mix_Music* music)
{
	if (!music)
		return;

	Mix_FreeMusic(music);
}

bool CAudioHandler::PlayMusic(Mix_Music* music, const int32_t numLoops)
{
	if (!music)
		return false;

	if (numLoops < -1)
	{
		if (Mix_PlayMusic(music, -1) == -1)
		{
		#if defined(_DEBUG)
			std::cout << "Error: failed to play music" << std::endl;
			std::cout << Mix_GetError() << std::endl;
		#endif

			return false;
		}
	}

	else
	{
		if (Mix_PlayMusic(music, numLoops) == -1)
		{
		#if defined(_DEBUG)
			std::cout << "Error: failed to play music" << std::endl;
			std::cout << Mix_GetError() << std::endl;
		#endif

			return false;
		}
	}

	return true;
}

bool CAudioHandler::StopMusic()
{
	if (Mix_PlayingMusic() == 0)
		return false;

	Mix_HaltMusic();

	return true;
}

void CAudioHandler::SetMusicVolume(const int32_t volume)
{
	if (volume < 0)
	{
		Mix_VolumeMusic(0);

		return;
	}

	Mix_VolumeMusic(volume);
}