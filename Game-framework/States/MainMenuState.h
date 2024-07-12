#pragma once

#include "GameObjects/Button.h"
#include "State.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class MainMenuState final : public State
{
public:

	 MainMenuState(void)													{}
	 MainMenuState(Application* mainApplication) : State(mainApplication)	{}
	~MainMenuState(void)													{}

	virtual bool OnEnter(void) override;//why do we need to override here?
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the main menu objects here and then create/destroy them in the OnEnter- and OnExit functions

	SDL_Texture*	menuBackground	= nullptr;
	SDL_Texture*	spider			= nullptr;

	TTF_Font*		menuFont		= nullptr;
	TTF_Font*		buttonMenuFont	= nullptr;

	Button*			titleTextBlock	= nullptr;
	Button*			playButton		= nullptr;
	Button*			quitButton		= nullptr;

	Mix_Music*		music			= nullptr;

	SDL_FPoint		spiderWebStart	= {0.0f, 0.0f};  
	SDL_FPoint		spiderSize		= {0.0f, 0.0f};
	SDL_FPoint		spiderPosition	= {0.0f, 0.0f};

	float			lifeTime		= 0.0f;
	float			spiderAngle		= 0.0f;

};