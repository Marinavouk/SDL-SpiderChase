#include "Pch.h"
#include "SettingsState.h"

#include "Application.h"

bool CSettingsState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering settings state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowCenter	= m_pApplication->GetWindowCenter();
	CWindow&			rWindow			= m_pApplication->GetWindow();

	rWindow.SetClearColor({0, 0, 0, 255});

	// Create objects that should be created/started when this state is entered/started (create textures and buttons, load/start main-menu music etc)

	m_pBackground = textureHandler.CreateTexture("menu_background.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(100);

	m_pTitleTextFont	= fontHandler.CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf",	200); if (!m_pTitleTextFont)	return false;
	m_pTextFont			= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	 50); if (!m_pTextFont)			return false;
	m_pButtonFont		= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",  30); if (!m_pButtonFont)		return false;

	const SDL_Color titleTextColor					= {200, 0,		0,		255}; // Dark red
	const SDL_Color buttonBackgroundColor			= {100, 100,	100,	150}; // Light gray	<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor	= {100, 100,	100,	200}; // Dark gray	<-- Background color when the button is held
	const SDL_Color buttonTextColor					= {255, 255,	255,	255}; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered			= {255, 0,		0,		255}; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed			= {255, 0,		0,		255}; // Red		<-- Text color when the button is held

	if (!m_TitleTextBlock.Create(m_pApplication, m_pTitleTextFont, "Settings", titleTextColor))
		return false;
	m_TitleTextBlock.SetPosition({windowCenter.x, 160.0f});
	m_TitleTextBlock.SetBackgroundColor({0, 0, 0, 0});

	if (!m_WindowModeTextBlock.Create(m_pApplication, m_pTextFont, "Window mode", titleTextColor))
		return false;
	m_WindowModeTextBlock.SetPosition({windowCenter.x, 350.0f});
	m_WindowModeTextBlock.SetBackgroundColor({0, 0, 0, 0});

	if (!m_WindowResizableTextBlock.Create(m_pApplication, m_pTextFont, "Window resizable", titleTextColor))
		return false;
	m_WindowResizableTextBlock.SetPosition({windowCenter.x, 530.0f});
	m_WindowResizableTextBlock.SetBackgroundColor({0, 0, 0, 0});

	const bool windowFullscreen = rWindow.GetFullscreen();
	const bool windowResizable	= rWindow.GetResizable();

	if (!m_FullscreenOnButton.Create(m_pApplication, m_pButtonFont, "Fullscreen", buttonTextColor))
		return false;
	m_FullscreenOnButton.SetPosition({windowCenter.x, 400.0f});
	m_FullscreenOnButton.SetBackgroundColor(buttonBackgroundColor);
	m_FullscreenOnButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_FullscreenOnButton.SetTextColorHovered(buttonTextColorHovered);
	m_FullscreenOnButton.SetTextColorPressed(buttonTextColorPressed);
	m_FullscreenOnButton.SetEnabled(!windowFullscreen);

	if (!m_FullscreenOffButton.Create(m_pApplication, m_pButtonFont, "Windowed", buttonTextColor))
		return false;
	m_FullscreenOffButton.SetPosition({windowCenter.x, 450});
	m_FullscreenOffButton.SetBackgroundColor(buttonBackgroundColor);
	m_FullscreenOffButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_FullscreenOffButton.SetTextColorHovered(buttonTextColorHovered);
	m_FullscreenOffButton.SetTextColorPressed(buttonTextColorPressed);
	m_FullscreenOffButton.SetEnabled(windowFullscreen);

	if (!m_WindowResizableOnButton.Create(m_pApplication, m_pButtonFont, "Enabled", buttonTextColor))
		return false;
	m_WindowResizableOnButton.SetPosition({windowCenter.x, 580.0f});
	m_WindowResizableOnButton.SetBackgroundColor(buttonBackgroundColor);
	m_WindowResizableOnButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_WindowResizableOnButton.SetTextColorHovered(buttonTextColorHovered);
	m_WindowResizableOnButton.SetTextColorPressed(buttonTextColorPressed);
	m_WindowResizableOnButton.SetEnabled(!windowResizable);

	if (!m_WindowResizableOffButton.Create(m_pApplication, m_pButtonFont, "Disabled", buttonTextColor))
		return false;
	m_WindowResizableOffButton.SetPosition({windowCenter.x, 630});
	m_WindowResizableOffButton.SetBackgroundColor(buttonBackgroundColor);
	m_WindowResizableOffButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_WindowResizableOffButton.SetTextColorHovered(buttonTextColorHovered);
	m_WindowResizableOffButton.SetTextColorPressed(buttonTextColorPressed);
	m_WindowResizableOffButton.SetEnabled(windowResizable);

	if (!m_BackButton.Create(m_pApplication, m_pButtonFont, "Back", buttonTextColor))
		return false;
	const SDL_FPoint buttonSize = m_BackButton.GetSize();
	m_BackButton.SetPosition({(buttonSize.x * 0.5f) + 10.0f, m_pApplication->GetWindowSize().y - ((m_BackButton.GetSize().y * 0.5f) + 10.0f)});
	m_BackButton.SetBackgroundColor(buttonBackgroundColor);
	m_BackButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_BackButton.SetTextColorHovered(buttonTextColorHovered);
	m_BackButton.SetTextColorPressed(buttonTextColorPressed);

	return true;
}

void CSettingsState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting settings state" << std::endl;
#endif

	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main-menu music etc)

	m_BackButton.Destroy(m_pApplication);
	m_WindowResizableOffButton.Destroy(m_pApplication);
	m_WindowResizableOnButton.Destroy(m_pApplication);
	m_FullscreenOffButton.Destroy(m_pApplication);
	m_FullscreenOnButton.Destroy(m_pApplication);
	m_WindowResizableTextBlock.Destroy(m_pApplication);
	m_WindowModeTextBlock.Destroy(m_pApplication);
	m_TitleTextBlock.Destroy(m_pApplication);

	fontHandler.DestroyFont(m_pButtonFont);
	fontHandler.DestroyFont(m_pTextFont);
	fontHandler.DestroyFont(m_pTitleTextFont);
	m_pButtonFont		= nullptr;
	m_pTextFont			= nullptr;
	m_pTitleTextFont	= nullptr;
}

void CSettingsState::Update(const float deltaTime)
{
	CInputHandler& inputHandler = m_pApplication->GetInputHandler();
	const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

	// Update the main-menu objects here

	m_FullscreenOnButton.Update(inputHandler);
	m_FullscreenOffButton.Update(inputHandler);
	m_WindowResizableOnButton.Update(inputHandler);
	m_WindowResizableOnButton.Update(inputHandler);
	m_WindowResizableOffButton.Update(inputHandler);
	m_BackButton.Update(inputHandler);

	if (m_FullscreenOnButton.IsPressed(inputHandler) || m_FullscreenOffButton.IsPressed(inputHandler))
	{
		m_pApplication->GetWindow().ToggleFullscreen();

		m_FullscreenOnButton.SetEnabled(!m_FullscreenOnButton.GetEnabled());
		m_FullscreenOffButton.SetEnabled(!m_FullscreenOffButton.GetEnabled());
	}

	else if (m_WindowResizableOnButton.IsPressed(inputHandler) || m_WindowResizableOffButton.IsPressed(inputHandler))
	{
		m_pApplication->GetWindow().ToggleResizable();

		m_WindowResizableOnButton.SetEnabled(!m_WindowResizableOnButton.GetEnabled());
		m_WindowResizableOffButton.SetEnabled(!m_WindowResizableOffButton.GetEnabled());
	}

	// Switch state whenever any of the buttons- or a specific key on the keyboard is pressed
	if (m_BackButton.IsPressed(inputHandler))
		m_pApplication->SetState(CApplication::EState::MAIN_MENU);
}

void CSettingsState::Render(void)
{
	// It's always good practice to create local variables for data that is used in multiple places in a function
	// In this case the renderer for example is used in multiple places below
	// By having a local variable like this, m_pApplication->GetWindow()->GetRenderer() isn't called multiple times
	// This is an optimization and also reduces repetitive code
	SDL_Renderer*		renderer		= m_pApplication->GetWindow().GetRenderer();
	const SDL_FPoint	mousePosition	= m_pApplication->GetInputHandler().GetMousePosition();

	// Render the main-menu objects here

	m_pBackground->Render({0.0f, 0.0f});

	m_TitleTextBlock.Render(renderer);
	m_WindowModeTextBlock.Render(renderer);
	m_WindowResizableTextBlock.Render(renderer);

	m_FullscreenOnButton.Render(renderer, &mousePosition);
	m_FullscreenOffButton.Render(renderer, &mousePosition);
	m_WindowResizableOnButton.Render(renderer, &mousePosition);
	m_WindowResizableOffButton.Render(renderer, &mousePosition);
	m_BackButton.Render(renderer, &mousePosition);
}