#include "Pch.h"
#include "MainMenuState.h"

#include "Application.h"
#include "Globals.h"

bool CMainMenuState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering menu state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowCenter	= m_pApplication->GetWindowCenter();

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	m_pApplication->GetWindow().SetClearColor({0, 0, 0, 255});

	// Create objects that should be created/started when this state is entered/started (create textures and buttons, load/start main-menu music etc)

	m_pBackground = textureHandler.CreateTexture("menu_background.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(100);

	m_pSpider = textureHandler.CreateTexture("spider.png");
	m_pSpider->SetSize({64.0f, 64.0f});
	m_pSpider->SetTextureCoords(0, 64, 256, 320);

	m_pTextFont		= fontHandler.CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf",	200); if (!m_pTextFont)		return false;
	m_pButtonFont	= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	 60); if (!m_pButtonFont)	return false;

	const SDL_Color titleTextColor					= {200,	0,		0,		255}; // Dark red
	const SDL_Color buttonBackgroundColor			= {100,	100,	100,	150}; // Light gray	<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor	= {100,	100,	100,	200}; // Dark gray	<-- Background color when the button is held
	const SDL_Color buttonTextColor					= {255, 255,	255,	255}; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered			= {255,	0,		0,		255}; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed			= {255,	0,		0,		255}; // Red		<-- Text color when the button is held

	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_TitleTextBlock.Create(m_pApplication, m_pTextFont, "Spider Chase", titleTextColor))
		return false;
	m_TitleTextBlock.SetPosition({windowCenter.x, 160.0f});
	m_TitleTextBlock.SetBackgroundColor({0, 0, 0, 0}); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)

	if (!m_PlayButton.Create(m_pApplication, m_pButtonFont, "Play", buttonTextColor))
		return false;
	m_PlayButton.SetPosition({windowCenter.x, windowCenter.y + 100.0f});
	m_PlayButton.SetBackgroundColor(buttonBackgroundColor);
	m_PlayButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_PlayButton.SetTextColorHovered(buttonTextColorHovered);
	m_PlayButton.SetTextColorPressed(buttonTextColorPressed);

	if (!m_SettingsButton.Create(m_pApplication, m_pButtonFont, "Settings", buttonTextColor))
		return false;
	m_SettingsButton.SetPosition({windowCenter.x, windowCenter.y + 200.0f});
	m_SettingsButton.SetBackgroundColor(buttonBackgroundColor);
	m_SettingsButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_SettingsButton.SetTextColorHovered(buttonTextColorHovered);
	m_SettingsButton.SetTextColorPressed(buttonTextColorPressed);

	if (!m_QuitButton.Create(m_pApplication, m_pButtonFont, "Quit", buttonTextColor))
		return false;
	m_QuitButton.SetPosition({windowCenter.x, windowCenter.y + 300.0f});
	m_QuitButton.SetBackgroundColor(buttonBackgroundColor);
	m_QuitButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_QuitButton.SetTextColorHovered(buttonTextColorHovered);
	m_QuitButton.SetTextColorPressed(buttonTextColorPressed);

	m_SpiderWebStart	= {150.0f, 220.0f};
	m_SpiderSize		= {64.0f, 64.0f};
	m_SpiderPosition	= {m_SpiderWebStart.x - (m_SpiderSize.x * 0.5f), m_SpiderWebStart.y + 200.0f};

	m_LifeTime		= 0.0f;
	m_SpiderAngle	= 0.0f;

	if (m_pApplication->GetLastState() != CApplication::EState::SETTINGS)
	{
		e_pMusic = audioHandler.CreateMusic("Assets/Audio/menu.mp3");
		if (!e_pMusic)
			return false;

		audioHandler.PlayMusic(e_pMusic, -1);
		audioHandler.SetMusicVolume(0);
	}

	return true;
}

void CMainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main-menu music etc)

	if (m_pApplication->GetNextState() != CApplication::EState::SETTINGS)
	{
		CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

		audioHandler.StopMusic();
		audioHandler.DestroyMusic(e_pMusic);
		e_pMusic = nullptr;
	}

	m_QuitButton.Destroy(m_pApplication);
	m_SettingsButton.Destroy(m_pApplication);
	m_PlayButton.Destroy(m_pApplication);
	m_TitleTextBlock.Destroy(m_pApplication);

	fontHandler.DestroyFont(m_pButtonFont);
	fontHandler.DestroyFont(m_pTextFont);
	m_pButtonFont	= nullptr;
	m_pTextFont		= nullptr;

	textureHandler.DestroyTexture(m_pSpider->GetName());
	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pSpider		= nullptr;
	m_pBackground	= nullptr;
}

void CMainMenuState::Update(const float deltaTime)
{
	// Easy access to the input handler and the transition renderer, so you don't have to write m_pApplication->GetX() multiple times below
	CInputHandler&				inputHandler		= m_pApplication->GetInputHandler();
	const CTransitionRenderer&	transitionRenderer	= m_pApplication->GetTransitionRenderer();

	// Update the main-menu objects here

	m_PlayButton.Update(inputHandler);
	m_QuitButton.Update(inputHandler);
	m_SettingsButton.Update(inputHandler);

	// Switch state whenever any of the buttons are pressed
		 if (m_PlayButton.IsPressed(inputHandler))		m_pApplication->SetState(CApplication::EState::GAME);
	else if (m_SettingsButton.IsPressed(inputHandler))	m_pApplication->SetState(CApplication::EState::SETTINGS);
	else if (m_QuitButton.IsPressed(inputHandler))		m_pApplication->SetState(CApplication::EState::QUIT);

	m_LifeTime += deltaTime;

	m_SpiderAngle = sinf(m_LifeTime * 1.5f) * 10.0f;
	m_SpiderPosition.x = (m_SpiderWebStart.x - (m_SpiderSize.x * 0.5f)) + m_SpiderAngle;
	m_SpiderPosition.y = (m_SpiderWebStart.y + 200.0f) + (cosf((m_LifeTime * 0.5f) + (m_SpiderAngle * 0.1f)) * 30.0f);

	m_pSpider->SetAngle(-m_SpiderAngle);

	// Will only fade in/out the menu music if the game isn't switching to/from the settings state
	if (transitionRenderer.IsTransitioning() && (m_pApplication->GetNextState() != CApplication::EState::SETTINGS) && (m_pApplication->GetLastState() != CApplication::EState::SETTINGS))
		m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));
}

void CMainMenuState::Render(void)
{
	// It's always good practice to create local variables for data that is used in multiple places in a function
	// In this case the renderer for example is used in multiple places below
	// By having a local variable like this, m_pApplication->GetWindow()->GetRenderer() isn't called multiple times
	// This is an optimization and also reduces repetitive code
	SDL_Renderer*		renderer		= m_pApplication->GetWindow().GetRenderer();
	const SDL_FPoint	mousePosition	= m_pApplication->GetInputHandler().GetMousePosition();

	// Render the main-menu objects here

	m_pBackground->Render({0.0f, 0.0f});

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
	SDL_RenderDrawLineF(renderer, m_SpiderWebStart.x, m_SpiderWebStart.y, m_SpiderPosition.x + (m_SpiderSize.x * 0.5f), m_SpiderPosition.y + 30.0f);

	m_pSpider->Render(m_SpiderPosition);

	m_TitleTextBlock.Render(renderer);

	m_PlayButton.Render(renderer, &mousePosition);
	m_QuitButton.Render(renderer, &mousePosition);
	m_SettingsButton.Render(renderer, &mousePosition);
}