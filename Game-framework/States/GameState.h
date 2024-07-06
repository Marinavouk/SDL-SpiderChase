#pragma once

#include "GameObjects/Button.h"
#include "State.h"

#include <SDL_mixer.h>

class GameState final : public State
{
public:

	 GameState(void) {}
	~GameState(void) {}

	virtual bool Create(Application* mainApplication) override;
	virtual void Destroy(void) override;
	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the game objects here and then create/destroy them in the OnEnter- and OnExit functions

	TTF_Font*	buttonFont	= nullptr;

	SDL_Texture* texture	= nullptr;

	Button*		menuButton	= nullptr;

	Mix_Music*	music		= nullptr;

};