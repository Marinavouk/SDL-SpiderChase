#include "Pch.h"
#include "EndOfRoundState.h"

#include "Application.h"
#include "Globals.h"

bool CEndOfRoundState::OnEnter(void)
{
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowCenter	= m_pApplication->GetWindowCenter();

	m_pApplication->GetWindow().SetClearColor({0, 0, 0, 255});

	// Create objects that should be created/started when this state is entered/started (create textures, load/start the end-of-round music etc)

	m_pBackground = textureHandler.CreateTexture("menu_background.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(100);

	m_pTitleTextFont	= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	100); if (!m_pTitleTextFont)	return false;
	m_pSpiderCountFont	= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	 50); if (!m_pSpiderCountFont)	return false;
	m_pButtonFont		= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	 60); if (!m_pButtonFont)		return false;

	const SDL_Color titleTextColor					= {200, 0,		0,		255}; // Dark red
	const SDL_Color buttonBackgroundColor			= {100, 100,	100,	150}; // Light gray	<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor	= {100, 100,	100,	200}; // Dark gray	<-- Background color when the button is held
	const SDL_Color buttonTextColor					= {255, 255,	255,	255}; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered			= {255, 0,		0,		255}; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed			= {255, 0,		0,		255}; // Red		<-- Text color when the button is held

	if (!m_TitleTextBlock.Create(m_pApplication, m_pTitleTextFont, (e_EndOfRoundPlayerKilled ? "You have been eaten!" : "Time is up!"), titleTextColor))
		return false;
	m_TitleTextBlock.SetPosition({windowCenter.x, 130.0f});
	m_TitleTextBlock.SetBackgroundColor({0, 0, 0, 0});

	const std::string spiderString		= ((e_SpiderCount	> 1) ? " spiders!"													: " spider!");
	const std::string spiderCountString = (e_SpiderCount	> 0) ? "You killed " + std::to_string(e_SpiderCount) + spiderString	: "No spiders killed, better luck next time!";

	if (!m_ScoreTextBlock.Create(m_pApplication, m_pSpiderCountFont, spiderCountString, titleTextColor))
		return false;
	m_ScoreTextBlock.SetPosition({windowCenter.x, windowCenter.y - 60.0f});
	m_ScoreTextBlock.SetBackgroundColor({0, 0, 0, 0});

	if (!m_RestartButton.Create(m_pApplication, m_pButtonFont, "Restart", buttonTextColor))
		return false;
	m_RestartButton.SetPosition({windowCenter.x, windowCenter.y + 100.0f});
	m_RestartButton.SetBackgroundColor(buttonBackgroundColor);
	m_RestartButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_RestartButton.SetTextColorHovered(buttonTextColorHovered);
	m_RestartButton.SetTextColorPressed(buttonTextColorPressed);

	if (!m_MainMenuButton.Create(m_pApplication, m_pButtonFont, "Main menu", buttonTextColor))
		return false;
	m_MainMenuButton.SetPosition({windowCenter.x, windowCenter.y + 200.0f});
	m_MainMenuButton.SetBackgroundColor(buttonBackgroundColor);
	m_MainMenuButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_MainMenuButton.SetTextColorHovered(buttonTextColorHovered);
	m_MainMenuButton.SetTextColorPressed(buttonTextColorPressed);

	if (!m_QuitButton.Create(m_pApplication, m_pButtonFont, "Quit", buttonTextColor))
		return false;
	m_QuitButton.SetPosition({windowCenter.x, windowCenter.y + 300.0f});
	m_QuitButton.SetBackgroundColor(buttonBackgroundColor);
	m_QuitButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_QuitButton.SetTextColorHovered(buttonTextColorHovered);
	m_QuitButton.SetTextColorPressed(buttonTextColorPressed);

	e_pMusic = audioHandler.CreateMusic("Assets/Audio/end_of_round.mp3");
	if (!e_pMusic)
		return false;

	audioHandler.PlayMusic(e_pMusic, -1);
	audioHandler.SetMusicVolume(0);

	return true;
}

void CEndOfRoundState::OnExit(void)
{
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop end-of-round music etc)

	audioHandler.StopMusic();
	audioHandler.DestroyMusic(e_pMusic);
	e_pMusic = nullptr;

	m_QuitButton.Destroy(m_pApplication);
	m_MainMenuButton.Destroy(m_pApplication);
	m_RestartButton.Destroy(m_pApplication);
	m_ScoreTextBlock.Destroy(m_pApplication);
	m_TitleTextBlock.Destroy(m_pApplication);

	fontHandler.DestroyFont(m_pButtonFont);
	fontHandler.DestroyFont(m_pSpiderCountFont);
	fontHandler.DestroyFont(m_pTitleTextFont);
	m_pButtonFont		= nullptr;
	m_pSpiderCountFont	= nullptr;
	m_pTitleTextFont	= nullptr;

	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pBackground = nullptr;
}

void CEndOfRoundState::Update(const float deltaTime)
{
	CInputHandler&				inputHandler		= m_pApplication->GetInputHandler();
	const CTransitionRenderer&	transitionRenderer	= m_pApplication->GetTransitionRenderer();

	// Update the end-of-round objects here

	m_RestartButton.Update(inputHandler);
	m_MainMenuButton.Update(inputHandler);
	m_QuitButton.Update(inputHandler);

		 if (m_RestartButton.IsPressed(inputHandler))	m_pApplication->SetState(CApplication::EState::GAME);
	else if (m_MainMenuButton.IsPressed(inputHandler))	m_pApplication->SetState(CApplication::EState::MAIN_MENU);
	else if (m_QuitButton.IsPressed(inputHandler))		m_pApplication->SetState(CApplication::EState::QUIT);

	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));
}

void CEndOfRoundState::Render(void)
{
	SDL_Renderer*		renderer		= m_pApplication->GetWindow().GetRenderer();
	const SDL_FPoint	mousePosition	= m_pApplication->GetInputHandler().GetMousePosition();

	// Render the end-of-rounds objects here

	m_pBackground->Render({0.0f, 0.0f});

	m_TitleTextBlock.Render(renderer);
	m_ScoreTextBlock.Render(renderer);

	m_RestartButton.Render(renderer, &mousePosition);
	m_MainMenuButton.Render(renderer, &mousePosition);
	m_QuitButton.Render(renderer, &mousePosition);
}