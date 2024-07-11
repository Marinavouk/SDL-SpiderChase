#include "MainMenuState.h"

#include "Application.h"

#include <iostream>

bool MainMenuState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering menu state" << std::endl;
#endif

	// Create objects that should be created/started when this state is entered/started (create textures and buttons, load/start main menu music etc)

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler* textureHandler	= application->GetTextureHandler();
	FontHandler*	fontHandler		= application->GetFontHandler();

	menuBackground = textureHandler->CreateTexture("Assets/Textures/menu_background.png");
	SDL_SetTextureBlendMode(menuBackground, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(menuBackground, 100);

	spider = textureHandler->CreateTexture("Assets/Textures/spider_spritesheet.png");

	menuFont		= fontHandler->CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf",		200); if (!menuFont)		return false;
	buttonMenuFont	= fontHandler->CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf",	 60); if (!buttonMenuFont)	return false;

	const SDL_FPoint	windowSize						= application->GetWindow()->GetSize();
	const SDL_FPoint	windowSizeHalf					= { windowSize.x * 0.5f, windowSize.y * 0.5f };
	const SDL_Color		titleTextColor					= {200,	0,		0,		255}; // Dark red
	const SDL_Color		buttonBackgroundColor			= {100,	100,	100,	150}; // Light gray	<-- Background color when the button is not held
	const SDL_Color		buttonBackgroundPressedColor	= {100,	100,	100,	200}; // Dark gray	<-- Background color when the button is held
	const SDL_Color		buttonTextColor					= {255, 255,	255,	255}; // White		<-- Text color when the mouse pointer is outside the button
	const SDL_Color		buttonTextColorHovered			= {255,	0,		0,		255}; // Red		<-- Text color when the mouse pointer is inside (hovering) the button
	const SDL_Color		buttonTextColorPressed			= {255,	0,		0,		255}; // Red		<-- Text color when the button is held

	// Buttons can be used as text blocks too, without mouse interaction
	titleTextBlock = new Button;
	if (!titleTextBlock->Create(application, menuFont, "Spider Chase", {255, 0, 0, 255}))
		return false;
	titleTextBlock->SetPosition({windowSizeHalf.x, 160.0f});
	titleTextBlock->SetBackgroundColor({0, 0, 0, 0});

	playButton = new Button;
	if (!playButton->Create(application, buttonMenuFont, "Play", buttonTextColor))
		return false;
	playButton->SetPosition({windowSizeHalf.x, windowSizeHalf.y + 100.0f});
	playButton->SetBackgroundColor(buttonBackgroundColor);
	playButton->SetBackgroundPressedColor(buttonBackgroundPressedColor);
	playButton->SetTextColorHovered(buttonTextColorHovered);
	playButton->SetTextColorPressed(buttonTextColorPressed);

	quitButton = new Button;
	if (!quitButton->Create(application, buttonMenuFont, "Quit", buttonTextColor))
		return false;
	quitButton->SetPosition({windowSizeHalf.x, windowSizeHalf.y + 200.0f});
	quitButton->SetBackgroundColor(buttonBackgroundColor);
	quitButton->SetBackgroundPressedColor(buttonBackgroundPressedColor);
	quitButton->SetTextColorHovered(buttonTextColorHovered);
	quitButton->SetTextColorPressed(buttonTextColorPressed);

	spiderWebStart	= {150.0f, 220.0f};
	spiderSize		= {32.0f, 32.0f};
	spiderPosition	= {spiderWebStart.x - (spiderSize.x * 0.5f), spiderWebStart.y + 200.0f};

	lifeTime	= 0.0f;
	spiderAngle = 0.0f;

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is optional
	application->GetWindow()->SetClearColor({0, 0, 0, 255});

	return true;
}

void MainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main menu music etc)

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler* textureHandler	= application->GetTextureHandler();
	FontHandler*	fontHandler		= application->GetFontHandler();

	quitButton->Destroy();
	delete quitButton;
	quitButton = nullptr;

	playButton->Destroy();
	delete playButton;
	playButton = nullptr;

	titleTextBlock->Destroy();
	delete titleTextBlock;
	titleTextBlock = nullptr;

	fontHandler->DestroyFont(buttonMenuFont);
	fontHandler->DestroyFont(menuFont);
	buttonMenuFont	= nullptr;
	menuFont		= nullptr;

	textureHandler->DestroyTexture(spider);
	textureHandler->DestroyTexture(menuBackground);
	spider			= nullptr;
	menuBackground	= nullptr;
}

void MainMenuState::Update(const float deltaTime)
{
	// Update all the needed main menu objects here

	// Easy access to the input handler so you don't have to write application->GetInputHandler() multiple times below
	InputHandler* inputHandler = application->GetInputHandler();

	playButton->Update(inputHandler);
	quitButton->Update(inputHandler);

	// Switch state whenever any of the buttons- or a specific key on the keyboard is pressed
		 if (playButton->IsPressed(inputHandler) || inputHandler->KeyPressed(SDL_SCANCODE_RETURN)) application->SetState(Application::EState::GAME);
	else if (quitButton->IsPressed(inputHandler) || inputHandler->KeyPressed(SDL_SCANCODE_ESCAPE)) application->SetState(Application::EState::QUIT);

	lifeTime += deltaTime;

	spiderAngle = sinf(lifeTime * 1.5f) * 10.0f;
	spiderPosition.x = (spiderWebStart.x - (spiderSize.x * 0.5f)) + spiderAngle;
	spiderPosition.y = (spiderWebStart.y + 200.0f) + (cosf((lifeTime * 0.5f) + (spiderAngle * 0.1f)) * 30.0f);
}

void MainMenuState::Render(void)
{
	// Render all the main menu objects here

	SDL_Renderer*		renderer		= application->GetWindow()->GetRenderer();
	TextureHandler*		textureHandler	= application->GetTextureHandler();
	const SDL_FPoint	mousePosition	= application->GetInputHandler()->GetMousePosition();
	const SDL_FPoint	windowSize		= application->GetWindow()->GetSize();
	const SDL_FRect		dstRect			= {0.0f, 0.0f, windowSize.x, windowSize.y};

	textureHandler->RenderTexture(menuBackground, {0.0f, 0.0f}, nullptr, &dstRect);

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 150);
	SDL_RenderDrawLineF(renderer, spiderWebStart.x, spiderWebStart.y, spiderPosition.x + (spiderSize.x * 0.5f), spiderPosition.y + spiderSize.y);

	const SDL_Rect	spiderClipRect	= {9, 118, 13, 10};
	const SDL_FRect	spiderDstRect	= {spiderPosition.x, spiderPosition.y, spiderSize.x, spiderSize.y};
	textureHandler->RenderTextureRotated(spider, spiderPosition, -spiderAngle, &spiderClipRect, &spiderDstRect);

	titleTextBlock->Render(renderer);
	playButton->Render(renderer, &mousePosition);
	quitButton->Render(renderer, &mousePosition);
}