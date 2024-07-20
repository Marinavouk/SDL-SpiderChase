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
	TextureHandler& textureHandler	= application->GetTextureHandler();
	FontHandler&	fontHandler		= application->GetFontHandler();
	AudioHandler&	audioHandler	= application->GetAudioHandler();

	menuBackground = textureHandler.CreateTexture("Assets/Textures/menu_background.png");
	if (!menuBackground)
		return false;

	SDL_SetTextureBlendMode(menuBackground, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(menuBackground, 100);

	spider = textureHandler.CreateTexture("Assets/Textures/spider.png");
	if (!spider)
		return false;

	menuFont		= fontHandler.CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf",	200); if (!menuFont)		return false;
	buttonMenuFont	= fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	 60); if (!buttonMenuFont)	return false;

	const SDL_FPoint	windowSize						= application->GetWindow().GetSize();
	const SDL_FPoint	windowSizeHalf					= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_Color		titleTextColor					= {200,	0,		0,		255}; // Dark red
	const SDL_Color		buttonBackgroundColor			= {100,	100,	100,	150}; // Light gray	<-- Background color when the button is not held
	const SDL_Color		buttonBackgroundPressedColor	= {100,	100,	100,	200}; // Dark gray	<-- Background color when the button is held
	const SDL_Color		buttonTextColor					= {255, 255,	255,	255}; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color		buttonTextColorHovered			= {255,	0,		0,		255}; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color		buttonTextColorPressed			= {255,	0,		0,		255}; // Red		<-- Text color when the button is held

	// Buttons can be used as text blocks too, without mouse interaction
	if (!titleTextBlock.Create(application, menuFont, "Spider Chase", {200, 0, 0, 255}))
		return false;
	titleTextBlock.SetPosition({windowSizeHalf.x, 160.0f});

	// Invisible background color (alpha = 0) since the text is the only thing that should be visible when rendering the text block
	titleTextBlock.SetBackgroundColor({0, 0, 0, 0});

	if (!playButton.Create(application, buttonMenuFont, "Play", buttonTextColor))
		return false;
	playButton.SetPosition({windowSizeHalf.x, windowSizeHalf.y + 100.0f});
	playButton.SetBackgroundColor(buttonBackgroundColor);
	playButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	playButton.SetTextColorHovered(buttonTextColorHovered);
	playButton.SetTextColorPressed(buttonTextColorPressed);

	if (!quitButton.Create(application, buttonMenuFont, "Quit", buttonTextColor))
		return false;
	quitButton.SetPosition({windowSizeHalf.x, windowSizeHalf.y + 200.0f});
	quitButton.SetBackgroundColor(buttonBackgroundColor);
	quitButton.SetBackgroundPressedColor(buttonBackgroundPressedColor);
	quitButton.SetTextColorHovered(buttonTextColorHovered);
	quitButton.SetTextColorPressed(buttonTextColorPressed);

	spiderWebStart	= {150.0f, 220.0f};
	spiderSize		= {64.0f, 64.0f};
	spiderPosition	= {spiderWebStart.x - (spiderSize.x * 0.5f), spiderWebStart.y + 200.0f};

	lifeTime	= 0.0f;
	spiderAngle = 0.0f;

	music = audioHandler.CreateMusic("Assets/Audio/menu.mp3");
	if (!music)
		return false;

	audioHandler.PlayMusic(music, -1);
	audioHandler.SetMusicVolume(0);

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	application->GetWindow().SetClearColor({0, 0, 0, 255});

	return true;
}

void MainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler& textureHandler	= application->GetTextureHandler();
	FontHandler&	fontHandler		= application->GetFontHandler();
	AudioHandler&	audioHandler	= application->GetAudioHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main menu music etc)

	audioHandler.StopMusic();
	audioHandler.DestroyMusic(music);
	music = nullptr;

	quitButton.Destroy();
	playButton.Destroy();
	titleTextBlock.Destroy();

	fontHandler.DestroyFont(buttonMenuFont);
	fontHandler.DestroyFont(menuFont);
	buttonMenuFont	= nullptr;
	menuFont		= nullptr;

	textureHandler.DestroyTexture(spider);
	textureHandler.DestroyTexture(menuBackground);
	spider			= nullptr;
	menuBackground	= nullptr;
}

void MainMenuState::Update(const float deltaTime)
{
	// Update all the needed main menu objects here

	// Easy access to the input handler so you don't have to write application->GetInputHandler() multiple times below
	InputHandler&				inputHandler		= application->GetInputHandler();
	const TransitionRenderer&	transitionRenderer	= application->GetTransitionRenderer();

	playButton.Update(inputHandler);
	quitButton.Update(inputHandler);

	// Switch state whenever any of the buttons- or a specific key on the keyboard is pressed
		 if (playButton.IsPressed(inputHandler) || inputHandler.KeyPressed(SDL_SCANCODE_RETURN)) application->SetState(Application::EState::GAME);
	else if (quitButton.IsPressed(inputHandler) || inputHandler.KeyPressed(SDL_SCANCODE_ESCAPE)) application->SetState(Application::EState::QUIT);

	lifeTime += deltaTime;

	spiderAngle = sinf(lifeTime * 1.5f) * 10.0f;
	spiderPosition.x = (spiderWebStart.x - (spiderSize.x * 0.5f)) + spiderAngle;
	spiderPosition.y = (spiderWebStart.y + 200.0f) + (cosf((lifeTime * 0.5f) + (spiderAngle * 0.1f)) * 30.0f);

	// Will fade the menu music in/out whenever the game switch to/from this state
	if (transitionRenderer.IsTransitioning())
		application->GetAudioHandler().SetMusicVolume(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer.GetTransitionValue()));
}

void MainMenuState::Render(void)
{
	// Render all the main menu objects here

	// It's always good practice to create local variables for data that is used in multiple places in a function, in this the renderer for example is used on multiple places below
	// By having a local variable like this, application->GetWindow()->GetRenderer(), application->GetTextureHandler() etc isn't called multiple times
	// This is both an optimization and also reduces repetitive code
	Window&				window			= application->GetWindow();
	TextureHandler&		textureHandler	= application->GetTextureHandler();
	SDL_Renderer*		renderer		= window.GetRenderer();
	const SDL_FPoint	mousePosition	= application->GetInputHandler().GetMousePosition();
	const SDL_FPoint	windowSize		= window.GetSize();

	textureHandler.RenderTexture(menuBackground, {0.0f, 0.0f}, nullptr, &windowSize);

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
	SDL_RenderDrawLineF(renderer, spiderWebStart.x, spiderWebStart.y, spiderPosition.x + (spiderSize.x * 0.5f), spiderPosition.y + 30.0f);

	const SDL_Rect spiderClipRect = {64, 260, 64, 64};
	textureHandler.RenderTextureRotated(spider, spiderPosition, -spiderAngle, &spiderClipRect, &spiderSize);

	titleTextBlock.Render(renderer);
	playButton.Render(renderer, &mousePosition);
	quitButton.Render(renderer, &mousePosition);
}