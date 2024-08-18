#pragma once

#include "GameObjects/Button.h"
#include "Handlers/TextureHandler.h"
#include "States/State.h"
#include "Utilities/Texture.h"
#include "GameObjects/Spider.h"

class CEndOfRoundState final : public CState
{
public:

	 CEndOfRoundState(void)												{}
	 CEndOfRoundState(CApplication* application) : CState(application)	{}
	~CEndOfRoundState(void)												{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	CTexture*	m_pBackground		= nullptr;

	TTF_Font*	m_pTitleTextFont	= nullptr;
	TTF_Font*	m_pSpiderCountFont	= nullptr;
	TTF_Font*	m_pButtonFont		= nullptr;

	CButton		m_TitleTextBlock	= {};
	CButton		m_ScoreTextBlock	= {};
	CButton		m_RestartButton		= {};
	CButton		m_MainMenuButton	= {};
	CButton		m_QuitButton		= {};

	bool		m_TimeUp			= false;

};