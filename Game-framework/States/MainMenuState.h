#pragma once

#include "State.h"

#include <SDL.h>

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

	SDL_Texture* menuBackground = nullptr;

};