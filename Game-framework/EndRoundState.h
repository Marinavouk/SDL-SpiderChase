#pragma once

#include "States/State.h"
#include "Utilities/Texture.h"
#include "Handlers/TextureHandler.h"
#include "GameObjects/Button.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class CEndRoundState final : public CState
{

public:
	CEndRoundState(void) {}
	CEndRoundState(CApplication* application) : CState(application) {}
	~CEndRoundState(void) {}

	virtual bool OnEnter(void) override;//why do we need to override here?
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	CTexture* m_pBackground = nullptr;

	TTF_Font* m_TextFont = nullptr;
	TTF_Font* m_ButtonFont = nullptr;

	Mix_Music* m_pMusic = nullptr;

	CButton		m_EndGameTextBlock = {};
	CButton		m_RestartButton = {};
	CButton		m_QuitButton = {};
};

