#pragma once

#include "GameObjects/Button.h"
#include "States/State.h"
#include "Utilities/Texture.h"

class CSettingsState final : public CState
{
public:

	CSettingsState(void)											{}
	CSettingsState(CApplication* application) : CState(application) {}
	~CSettingsState(void)											{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	CTexture*	m_pBackground				= nullptr;

	TTF_Font*	m_pTitleTextFont			= nullptr;
	TTF_Font*	m_pTextFont					= nullptr;
	TTF_Font*	m_pButtonFont				= nullptr;

	CButton		m_TitleTextBlock			= {};
	CButton		m_WindowModeTextBlock		= {};
	CButton		m_WindowResizableTextBlock	= {};
	CButton		m_FullscreenOnButton		= {};
	CButton		m_FullscreenOffButton		= {};
	CButton		m_WindowResizableOnButton	= {};
	CButton		m_WindowResizableOffButton	= {};
	CButton		m_BackButton				= {};
};

