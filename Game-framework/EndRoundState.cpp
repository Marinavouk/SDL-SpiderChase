#include "EndRoundState.h"

#include "Application.h"
#include "Handlers/AudioHandler.h"

#include <iostream>

bool CEndRoundState::OnEnter(void)
{
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();
	CWindow& window = m_pApplication->GetWindow();
	const SDL_FPoint	windowCenter = window.GetCenter();

	window.SetClearColor({ 0, 0, 0, 255 });

	m_pBackground = textureHandler.CreateTexture("menu_background.png");
	m_pBackground->SetSize(window.GetSize());
	m_pBackground->SetAlphaMod(100);

	m_TextFont = fontHandler.CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf", 200); if (!m_TextFont)		return false;
	m_ButtonFont = fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf", 60); if (!m_ButtonFont)	return false;

	const SDL_Color endGameTextColor = { 200,	0,		0,		255 }; // Dark red
	const SDL_Color buttonBackgroundColor = { 100,	100,	100,	150 }; // Light gray	<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 100,	100,	100,	200 }; // Dark gray	<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255, 255,	255,	255 }; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered = { 255,	0,		0,		255 }; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed = { 255,	0,		0,		255 }; // Red		<-- Text color when the button is held

	if (!m_EndGameTextBlock.Create(m_pApplication, m_TextFont, "You have been eaten!", { 200, 0, 0, 255 }))
		return false;
	m_EndGameTextBlock.SetPosition({ windowCenter.x, 160.0f });

	m_EndGameTextBlock.SetBackgroundColor({ 0, 0, 0, 0 });

	if (!m_RestartButton.Create(m_pApplication, m_ButtonFont, "Play", buttonTextColor))
		return false;
	m_RestartButton.SetPosition({ windowCenter.x, windowCenter.y + 100.0f });
	m_RestartButton.SetBackgroundColor(buttonBackgroundColor);
	m_RestartButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_RestartButton.SetTextColorHovered(buttonTextColorHovered);
	m_RestartButton.SetTextColorPressed(buttonTextColorPressed);

	if (!m_QuitButton.Create(m_pApplication, m_ButtonFont, "Quit", buttonTextColor))
		return false;
	m_QuitButton.SetPosition({ windowCenter.x, windowCenter.y + 200.0f });
	m_QuitButton.SetBackgroundColor(buttonBackgroundColor);
	m_QuitButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_QuitButton.SetTextColorHovered(buttonTextColorHovered);
	m_QuitButton.SetTextColorPressed(buttonTextColorPressed);

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/menu.mp3");
	if (!m_pMusic)
		return false;

	audioHandler.PlayMusic(m_pMusic, -1);
	audioHandler.SetMusicVolume(0);

	return true;
}

void CEndRoundState::OnExit(void)
{
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

	audioHandler.StopMusic();
	audioHandler.DestroyMusic(m_pMusic);
	m_pMusic = nullptr;

	m_QuitButton.Destroy(m_pApplication);
	m_RestartButton.Destroy(m_pApplication);
	m_EndGameTextBlock.Destroy(m_pApplication);

	fontHandler.DestroyFont(m_ButtonFont);
	fontHandler.DestroyFont(m_TextFont);
	m_ButtonFont = nullptr;
	m_TextFont = nullptr;

	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pBackground = nullptr;
}

void CEndRoundState::Update(const float deltaTime)
{
	CInputHandler& inputHandler = m_pApplication->GetInputHandler();
	const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

	m_RestartButton.Update(inputHandler);
	m_QuitButton.Update(inputHandler);

	if (m_RestartButton.IsPressed(inputHandler) || inputHandler.KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(CApplication::EState::GAME);
	else if (m_QuitButton.IsPressed(inputHandler) || inputHandler.KeyPressed(SDL_SCANCODE_ESCAPE)) m_pApplication->SetState(CApplication::EState::QUIT);

	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));

}

void CEndRoundState::Render(void)
{
}
