#pragma once

#include "States/State.h"
#include "Utilities/Texture.h"
#include "Handlers/TextureHandler.h"
#include "GameObjects/Button.h"


class CEndOfTimeState final : public CState
{

public:

	CEndOfTimeState(void) {}
	CEndOfTimeState(CApplication* application) : CState(application) {}
	~CEndOfTimeState(void) {}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	CTexture* m_pBackground = nullptr;

	TTF_Font* m_TextFont = nullptr;
	TTF_Font* m_ButtonFont = nullptr;

	Mix_Music* m_pMusic = nullptr;

	CButton		m_TitleTextBlock = {};
	CButton		m_RestartButton = {};
	CButton		m_MainMenuButton = {};
	CButton		m_QuitButton = {};
	CButton		m_Score = {};
};

