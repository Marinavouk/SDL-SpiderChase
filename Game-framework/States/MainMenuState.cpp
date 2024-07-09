#include "MainMenuState.h"

#include "Application.h"

#include <iostream>

bool MainMenuState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering menu state" << std::endl;
#endif

	// Create objects that should be created/started when this state is entered/started (create textures and buttons, load/start main menu music etc)

	// Create the menu background texture
	menuBackground = application->GetTextureHandler()->CreateTexture("Assets/Textures/menu_background.png");
	SDL_SetTextureBlendMode(menuBackground, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(menuBackground, 100);

	FontHandler* fontHandler = application->GetFontHandler();

	menuFont = fontHandler->CreateFont("Assets/Fonts/SpiderDemo-51LlB.ttf", 200);
	if (!menuFont)
		return false;

	buttonMenuFont = fontHandler->CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf", 45);
	if (!buttonMenuFont)
		return false;

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

	FontHandler* fontHandler = application->GetFontHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main menu music etc)
	fontHandler->DestroyFont(buttonMenuFont);
	buttonMenuFont = nullptr;

	fontHandler->DestroyFont(menuFont);
	menuFont = nullptr;

	// Destroy the menu background texture
	application->GetTextureHandler()->DestroyTexture(menuBackground);
	menuBackground = nullptr;
}

void MainMenuState::Update(const float deltaTime)
{
	// Update all the needed main menu objects here

	InputHandler* inputHandler = application->GetInputHandler();

	// If the escape key on the keyboard is pressed, shut down the game
	if (inputHandler->KeyPressed(SDL_SCANCODE_ESCAPE))
		application->SetState(Application::EState::QUIT);

	// If the enter (return) key on the keyboard is pressed, switch to the game state
	if (inputHandler->KeyPressed(SDL_SCANCODE_RETURN))
		application->SetState(Application::EState::GAME);
}

void MainMenuState::Render(void)
{
	// Render all the main menu objects here

	FontHandler*		fontHandler		= application->GetFontHandler();
	const std::string	titleText		= "Spider Chase";
	const SDL_FPoint	windowSize		= application->GetWindow()->GetSize();
	const SDL_FPoint	windowSizeHalf	= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_FPoint	textSize		= fontHandler->GetTextSize(menuFont, titleText);
	const SDL_FPoint	textPosition	= {windowSizeHalf.x - (textSize.x * 0.5f), 50.0f};
	const SDL_FRect		dstRect			= {0.0f, 0.0f, windowSize.x, windowSize.y};
	const SDL_Color		titleTextColor	= {200, 0, 0, 255}; // Red text

	application->GetTextureHandler()->RenderTexture(menuBackground, {0.0f, 0.0f}, nullptr, &dstRect);

	fontHandler->RenderText(application->GetWindow()->GetRenderer(), menuFont, titleText, textPosition, titleTextColor);
}