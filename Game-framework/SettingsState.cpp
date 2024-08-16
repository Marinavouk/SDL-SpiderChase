#include "Pch.h"

#include "SettingsState.h"
#include "Application.h"
#include "Handlers/AudioHandler.h"

bool CSettingsState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering settings state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowCenter = m_pApplication->GetWindowCenter();

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	m_pApplication->GetWindow().SetClearColor({ 0, 0, 0, 255 });

	// Create objects that should be created/started when this state is entered/started (create textures and buttons, load/start main-menu music etc)

	m_pBackground = textureHandler.CreateTexture("menu_background.png");
	m_pBackground->SetSize(m_pApplication->GetWindowSize());
	m_pBackground->SetAlphaMod(100);

	m_pTextFont = fontHandler.CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf", 200); if (!m_pTextFont)		return false;
	m_pButtonFont = fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf", 60); if (!m_pButtonFont)	return false;

	const SDL_Color titleTextColor = { 200,	0,		0,		255 }; // Dark red
	const SDL_Color buttonBackgroundColor = { 100,	100,	100,	150 }; // Light gray	<-- Background color when the button is not held
	const SDL_Color buttonBackgroundPressedColor = { 100,	100,	100,	200 }; // Dark gray	<-- Background color when the button is held
	const SDL_Color buttonTextColor = { 255, 255,	255,	255 }; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color buttonTextColorHovered = { 255,	0,		0,		255 }; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color buttonTextColorPressed = { 255,	0,		0,		255 }; // Red		<-- Text color when the button is held

	// Buttons can be used as text blocks too, without mouse interaction
	if (!m_TitleTextBlock.Create(m_pApplication, m_pTextFont, "Settings Menu", titleTextColor))
		return false;
	m_TitleTextBlock.SetPosition({ windowCenter.x, 160.0f });
	m_TitleTextBlock.SetBackgroundColor({ 0, 0, 0, 0 }); // Only the text in the text block should be visible, so the background is set to be invisible (alpha = 0)


	if (!m_BackButton.Create(m_pApplication, m_pButtonFont, "Back", buttonTextColor))
		return false;
	m_BackButton.SetPosition({ windowCenter.x, windowCenter.y + 200.0f });
	m_BackButton.SetBackgroundColor(buttonBackgroundColor);
	m_BackButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	m_BackButton.SetTextColorHovered(buttonTextColorHovered);
	m_BackButton.SetTextColorPressed(buttonTextColorPressed);

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/menu.mp3");
	if (!m_pMusic)
		return false;

	//	audioHandler.PlayMusic(m_pMusic, -1);
	audioHandler.SetMusicVolume(0);

	return true;
}

void CSettingsState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting settings state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler& textureHandler = m_pApplication->GetTextureHandler();
	CFontHandler& fontHandler = m_pApplication->GetFontHandler();
	CAudioHandler& audioHandler = m_pApplication->GetAudioHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main-menu music etc)

	audioHandler.StopMusic();
	audioHandler.DestroyMusic(m_pMusic);
	m_pMusic = nullptr;

	m_BackButton.Destroy(m_pApplication);
	m_TitleTextBlock.Destroy(m_pApplication);

	fontHandler.DestroyFont(m_pButtonFont);
	fontHandler.DestroyFont(m_pTextFont);
	m_pButtonFont = nullptr;
	m_pTextFont = nullptr;
}

void CSettingsState::Update(const float deltaTime)
{
	CInputHandler& inputHandler = m_pApplication->GetInputHandler();
	const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

	// Update the main-menu objects here

	m_BackButton.Update(inputHandler);
	
	// Switch state whenever any of the buttons- or a specific key on the keyboard is pressed
	if (m_BackButton.IsPressed(inputHandler) || inputHandler.KeyPressed(SDL_SCANCODE_RETURN)) m_pApplication->SetState(CApplication::EState::MAIN_MENU);
	
	// Will fade the menu music in/out whenever the game switch to/from this state
	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));

}

void CSettingsState::Render(void)
{
	// It's always good practice to create local variables for data that is used in multiple places in a function
	// In this case the renderer for example is used in multiple places below
	// By having a local variable like this, m_pApplication->GetWindow()->GetRenderer() isn't called multiple times
	// This is an optimization and also reduces repetitive code
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();
	const SDL_FPoint	mousePosition = m_pApplication->GetInputHandler().GetMousePosition();

	// Render the main-menu objects here

	m_pBackground->Render({ 0.0f, 0.0f });

	m_TitleTextBlock.Render(renderer);

	m_BackButton.Render(renderer, &mousePosition);
}
