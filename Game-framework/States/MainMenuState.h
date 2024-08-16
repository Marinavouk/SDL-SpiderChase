#pragma once

#include "GameObjects/Button.h"
#include "State.h"
#include "Utilities/Texture.h"

class CMainMenuState final : public CState
{
public:

	 CMainMenuState(void)												{}
	 CMainMenuState(CApplication* application) : CState(application)	{}
	~CMainMenuState(void)												{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the main menu objects here and then create/destroy them in the OnEnter- and OnExit functions

	CTexture*	m_pBackground		= nullptr;
	CTexture*	m_pSpider			= nullptr;

	TTF_Font*	m_pTextFont			= nullptr;
	TTF_Font*	m_pButtonFont		= nullptr;

	Mix_Music*	m_pMusic			= nullptr;

	CButton		m_TitleTextBlock	= {};
	CButton		m_PlayButton		= {};
	CButton		m_QuitButton		= {};
	CButton		m_SettingsButton	= {};

	SDL_FPoint	m_SpiderWebStart	= {0.0f, 0.0f};  
	SDL_FPoint	m_SpiderSize		= {0.0f, 0.0f};
	SDL_FPoint	m_SpiderPosition	= {0.0f, 0.0f};

	float		m_LifeTime			= 0.0f;
	float		m_SpiderAngle		= 0.0f;

};