#pragma once

#include "State.h"

#include <SDL.h>
#include <SDL_ttf.h>

class MainMenuState final : public State
{
public:

	 MainMenuState(void)													{}
	 MainMenuState(Application* mainApplication) : State(mainApplication)	{}
	~MainMenuState(void)													{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the main menu objects here and then create/destroy them in the OnEnter- and OnExit functions

	SDL_Texture*	menuBackground	= nullptr;

	TTF_Font*		menuFont		= nullptr;

	TTF_Font*		buttonMenuFont	= nullptr;

};