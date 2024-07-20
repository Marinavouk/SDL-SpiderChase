#include "MainMenuState.h"

#include "Application.h"
#include "Handlers/AudioHandler.h"

#include <iostream>

bool MainMenuState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering menu state" << std::endl;
#endif

	// Create objects that should be created/started when this state is entered/started (create textures and buttons, load/start main menu music etc)

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler& textureHandler	= m_pApplication->GetTextureHandler();
	FontHandler&	fontHandler		= m_pApplication->GetFontHandler();
	AudioHandler&	audioHandler	= m_pApplication->GetAudioHandler();

	m_pBackground = textureHandler.CreateTexture("Assets/Textures/menu_background.png");
	if (!m_pBackground)
		return false;

	SDL_SetTextureBlendMode(m_pBackground, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(m_pBackground, 100);

	m_pSpider = textureHandler.CreateTexture("Assets/Textures/spider.png");
	if (!m_pSpider)
		return false;

	m_TextFont		= fontHandler.CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf",	200); if (!m_TextFont)		return false;
	m_ButtonFont	= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	 60); if (!m_ButtonFont)	return false;

	const SDL_FPoint	windowSize						= m_pApplication->GetWindow().GetSize();
	const SDL_FPoint	windowSizeHalf					= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_Color		titleTextColor					= {200,	0,		0,		255}; // Dark red
	const SDL_Color		buttonBackgroundColor			= {100,	100,	100,	150}; // Light gray	<-- Background color when the button is not held
	const SDL_Color		buttonBackgroundPressedColor	= {100,	100,	100,	200}; // Dark gray	<-- Background color when the button is held
	const SDL_Color		buttonTextColor					= {255, 255,	255,	255}; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color		buttonTextColorHovered			= {255,	0,		0,		255}; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color		buttonTextColorPressed			= {255,	0,		0,		255}; // Red		<-- Text color when the button is held

	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_TitleTextBlock.Create(m_pApplication, m_TextFont, "Spider Chase", {200, 0, 0, 255}))
		return false;
	m_TitleTextBlock.SetPosition({windowSizeHalf.x, 160.0f});

	// Invisible background color (alpha = 0) since the text is the only thing that should be visible when rendering the text block
	m_TitleTextBlock.SetBackgroundColor({0, 0, 0, 0});

	if (!m_PlayButton.Create(m_pApplication, m_ButtonFont, "Play", buttonTextColor))
		return false;
	m_PlayButton.SetPosition({windowSizeHalf.x, windowSizeHalf.y + 100.0f});
	m_PlayButton.SetBackgroundColor(buttonBackgroundColor);
	m_PlayButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_PlayButton.SetTextColorHovered(buttonTextColorHovered);
	m_PlayButton.SetTextColorPressed(buttonTextColorPressed);

	if (!m_QuitButton.Create(m_pApplication, m_ButtonFont, "Quit", buttonTextColor))
		return false;
	m_QuitButton.SetPosition({windowSizeHalf.x, windowSizeHalf.y + 200.0f});
	m_QuitButton.SetBackgroundColor(buttonBackgroundColor);
	m_QuitButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_QuitButton.SetTextColorHovered(buttonTextColorHovered);
	m_QuitButton.SetTextColorPressed(buttonTextColorPressed);

	m_SpiderWebStart	= {150.0f, 220.0f};
	m_SpiderSize		= {64.0f, 64.0f};
	m_SpiderPosition	= {m_SpiderWebStart.x - (m_SpiderSize.x * 0.5f), m_SpiderWebStart.y + 200.0f};

	m_LifeTime	= 0.0f;
	m_SpiderAngle = 0.0f;

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/menu.mp3");
	if (!m_pMusic)
		return false;

	audioHandler.PlayMusic(m_pMusic, -1);
	audioHandler.SetMusicVolume(0);

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	m_pApplication->GetWindow().SetClearColor({0, 0, 0, 255});

	return true;
}

void MainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler& textureHandler	= m_pApplication->GetTextureHandler();
	FontHandler&	fontHandler		= m_pApplication->GetFontHandler();
	AudioHandler&	audioHandler	= m_pApplication->GetAudioHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main menu music etc)

	audioHandler.StopMusic();
	audioHandler.DestroyMusic(m_pMusic);
	m_pMusic = nullptr;

	m_QuitButton.Destroy();
	m_PlayButton.Destroy();
	m_TitleTextBlock.Destroy();

	fontHandler.DestroyFont(m_ButtonFont);
	fontHandler.DestroyFont(m_TextFont);
	m_ButtonFont	= nullptr;
	m_TextFont		= nullptr;

	textureHandler.DestroyTexture(m_pSpider);
	textureHandler.DestroyTexture(m_pBackground);
	m_pSpider		= nullptr;
	m_pBackground	= nullptr;
}

void MainMenuState::Update(const float deltaTime)
{
	// Update all the needed main menu objects here

	// Easy access to the input handler so you don't have to write application->GetInputHandler() multiple times below
	InputHandler&				inputHandler		= m_pApplication->GetInputHandler();
	const TransitionRenderer&	transitionRenderer	= m_pApplication->GetTransitionRenderer();

	m_PlayButton.Update(inputHandler);
	m_QuitButton.Update(inputHandler);

	// Switch state whenever any of the buttons- or a specific key on the keyboard is pressed
		 if (m_PlayButton.IsPressed(inputHandler) || inputHandler.KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(Application::EState::GAME);
	else if (m_QuitButton.IsPressed(inputHandler) || inputHandler.KeyPressed(SDL_SCANCODE_ESCAPE)) m_pApplication->SetState(Application::EState::QUIT);

	m_LifeTime += deltaTime;

	m_SpiderAngle = sinf(m_LifeTime * 1.5f) * 10.0f;
	m_SpiderPosition.x = (m_SpiderWebStart.x - (m_SpiderSize.x * 0.5f)) + m_SpiderAngle;
	m_SpiderPosition.y = (m_SpiderWebStart.y + 200.0f) + (cosf((m_LifeTime * 0.5f) + (m_SpiderAngle * 0.1f)) * 30.0f);

	// Will fade the menu music in/out whenever the game switch to/from this state
	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));
}

void MainMenuState::Render(void)
{
	// Render all the main menu objects here

	// It's always good practice to create local variables for data that is used in multiple places in a function, in this the renderer for example is used on multiple places below
	// By having a local variable like this, application->GetWindow()->GetRenderer(), application->GetTextureHandler() etc isn't called multiple times
	// This is both an optimization and also reduces repetitive code
	Window&				window			= m_pApplication->GetWindow();
	TextureHandler&		textureHandler	= m_pApplication->GetTextureHandler();
	SDL_Renderer*		renderer		= window.GetRenderer();
	const SDL_FPoint	mousePosition	= m_pApplication->GetInputHandler().GetMousePosition();
	const SDL_FPoint	windowSize		= window.GetSize();

	textureHandler.RenderTexture(m_pBackground, {0.0f, 0.0f}, nullptr, &windowSize);

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
	SDL_RenderDrawLineF(renderer, m_SpiderWebStart.x, m_SpiderWebStart.y, m_SpiderPosition.x + (m_SpiderSize.x * 0.5f), m_SpiderPosition.y + 30.0f);

	const SDL_Rect spiderClipRect = {64, 260, 64, 64};
	textureHandler.RenderTextureRotated(m_pSpider, m_SpiderPosition, -m_SpiderAngle, &spiderClipRect, &m_SpiderSize);

	m_TitleTextBlock.Render(renderer);
	m_PlayButton.Render(renderer, &mousePosition);
	m_QuitButton.Render(renderer, &mousePosition);
}