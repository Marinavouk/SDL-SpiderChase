#pragma once

#include "GameObjects/Button.h"
#include "State.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class MainMenuState final : public State
{
public:

	 MainMenuState(void)											{}
	 MainMenuState(Application* application) : State(application)	{}
	~MainMenuState(void)											{}

	virtual bool OnEnter(void) override;//why do we need to override here?
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the main menu objects here and then create/destroy them in the OnEnter- and OnExit functions

	SDL_Texture*	m_pBackground		= nullptr;
	SDL_Texture*	m_pSpider			= nullptr;

	TTF_Font*		m_TextFont			= nullptr;
	TTF_Font*		m_ButtonFont		= nullptr;

	Mix_Music*		m_pMusic			= nullptr;

	Button			m_TitleTextBlock	= {};
	Button			m_PlayButton		= {};
	Button			m_QuitButton		= {};

	SDL_FPoint		m_SpiderWebStart	= {0.0f, 0.0f};  
	SDL_FPoint		m_SpiderSize		= {0.0f, 0.0f};
	SDL_FPoint		m_SpiderPosition	= {0.0f, 0.0f};

	float			m_LifeTime			= 0.0f;
	float			m_SpiderAngle		= 0.0f;

};