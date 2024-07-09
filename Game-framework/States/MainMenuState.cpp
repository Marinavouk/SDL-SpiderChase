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
	menuBackground = application->GetTextureHandler()->CreateTexture("Assets/Textures/mainmenuSpiderChase.jpg");
	//need to change the picture here
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

	int textureWidth	= 0;
	int textureHeight	= 0;
	SDL_QueryTexture(menuBackground, nullptr, nullptr, &textureWidth, &textureHeight);

	const SDL_FPoint windowSize			= application->GetWindow()->GetSize();
	const SDL_FPoint windowSizeHalf		= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_FPoint textureSizeHalf	= {textureWidth * 0.5f, textureHeight * 0.5f};

	// Render the background texture
	// The window size is 1280x720 and the menu background size is 512x512, so I center it in the window
	application->GetTextureHandler()->RenderTexture(menuBackground, {windowSizeHalf.x - textureSizeHalf.x, windowSizeHalf.y - textureSizeHalf.y});
}