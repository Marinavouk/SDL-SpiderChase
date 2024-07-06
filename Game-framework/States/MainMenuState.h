#pragma once

#include "GameObjects/Button.h"
#include "State.h"

#include <SDL_mixer.h>

class MainMenuState final : public State
{
public:

	 MainMenuState(void) {}
	~MainMenuState(void) {}

	virtual bool Create(Application* mainApplication) override;
	virtual void Destroy(void) override;
	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the main menu objects here and then create/destroy them in the OnEnter- and OnExit functions

	TTF_Font*	buttonFont		= nullptr;

	Button*		playButton		= nullptr;
	Button*		quitButton		= nullptr;

	Mix_Music*	music			= nullptr;

	bool		musicFadeDone	= false;

};