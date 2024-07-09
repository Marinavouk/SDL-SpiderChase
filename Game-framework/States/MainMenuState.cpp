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

	// Set the clear color (the background color that is shown behind the menu background and other objects) to the same color as the menu texture's background color
	// This is optional
	application->GetWindow()->SetClearColor({201, 198, 183, 255});

	return true;
}

void MainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main menu music etc)

	// Destroy the menu background texture
	application->GetTextureHandler()->DestroyTexture(menuBackground);
	menuBackground = nullptr;
}

void MainMenuState::Update(const float deltaTime)
{
	// Update all the needed main menu objects here

	// If the escape key on the keyboard is pressed, shut down the game
	if (application->GetInputHandler()->KeyPressed(SDL_SCANCODE_ESCAPE))
		application->SetState(Application::EState::QUIT);

	// If the enter (return) key on the keyboard is pressed, switch to the game state
	if (application->GetInputHandler()->KeyPressed(SDL_SCANCODE_RETURN))
		application->SetState(Application::EState::GAME);
}

void MainMenuState::Render(void)
{
	// Render all the main menu objects here

	const SDL_FPoint	windowSize	= application->GetWindow()->GetSize();
	const SDL_FRect		dstRect		= {0.0f, 0.0f, windowSize.x, windowSize.y};

	application->GetTextureHandler()->RenderTexture(menuBackground, {0.0f, 0.0f}, nullptr, &dstRect);
}