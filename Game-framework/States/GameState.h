#pragma once

#include "State.h"

#include <SDL.h>
#include <SDL_mixer.h>

class GameState final : public State
{
public:

	 GameState(void)													{}
	 GameState(Application* mainApplication) : State(mainApplication)	{}
	~GameState(void)													{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the game objects here and then create/destroy them in the OnEnter- and OnExit functions

	SDL_Texture*	mainBackground	= nullptr;
	SDL_Texture*	table			= nullptr;
	SDL_Texture*	chair			= nullptr;
	SDL_Texture*	triangleTest	= nullptr;

	Mix_Music*		music			= nullptr;

	SDL_FPoint		tableSize		= {0.0f, 0.0f};
	SDL_FPoint		chairSize		= {0.0f, 0.0f};

	int				volumeLimiter	= 100;

};