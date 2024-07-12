#include "AudioHandler.h"

#include <iostream>

Mix_Chunk* AudioHandler::CreateSound(const std::string& fileName)
{
	Mix_Chunk* chunk = Mix_LoadWAV(fileName.c_str());

	if (!chunk)
	{
		std::cout << "Error: failed to load sound '" << fileName.c_str() << "'" << std::endl;
		std::cout << Mix_GetError() << std::endl;
	}

	return chunk;
}

Mix_Music* AudioHandler::CreateMusic(const std::string& fileName)
{
	Mix_Music* music = Mix_LoadMUS(fileName.c_str());

	if (!music)
	{
		std::cout << "Error: failed to load music '" << fileName.c_str() << "'" << std::endl;
		std::cout << Mix_GetError() << std::endl;
	}

	return music;
}

void AudioHandler::DestroySound(Mix_Chunk* chunk)
{
	if (!chunk)
		return;

	Mix_FreeChunk(chunk);
}

void AudioHandler::DestroyMusic(Mix_Music* music)
{
	if (!music)
		return;

	Mix_FreeMusic(music);
}

bool AudioHandler::PlayMusic(Mix_Music* music, const int numLoops)
{
	if (!music)
		return false;

	if (numLoops < -1)
	{
		if (Mix_PlayMusic(music, -1) == -1)
		{
			std::cout << "Error: failed to play music" << std::endl;
			std::cout << Mix_GetError() << std::endl;

			return false;
		}
	}

	else
	{
		if (Mix_PlayMusic(music, numLoops) == -1)
		{
			std::cout << "Error: failed to play music" << std::endl;
			std::cout << Mix_GetError() << std::endl;

			return false;
		}
	}

	return true;
}

bool AudioHandler::StopMusic()
{
	if (Mix_PlayingMusic() == 0)
		return false;

	Mix_HaltMusic();

	return true;
}

void AudioHandler::SetMusicVolume(const int volume)
{
	if (volume < 0)
	{
		Mix_VolumeMusic(0);

		return;
	}

	Mix_VolumeMusic(volume);
}