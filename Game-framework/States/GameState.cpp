#include "GameState.h"

#include "Application.h"

#include <iostream>

bool GameState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering game state" << std::endl;
#endif

	// Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)

	// TODO: create your game background here


	// The menu state has a light-gray background color so I just set it to some blue color to show you that you can set another color for each state, if wanted
	// This is optional
	application->GetWindow()->SetClearColor({150, 150, 200, 255});

	return true;
}

void GameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop game music etc)

	// TODO: destroy your game background here

}

void GameState::Update(const float deltaTime)
{
	// Update all the needed game objects here

	// If the escape key on the keyboard is pressed, switch to the main menu
	if (application->GetInputHandler()->KeyPressed(SDL_SCANCODE_ESCAPE))
		application->SetState(Application::EState::MAIN_MENU);
}

void GameState::Render(void)
{
	// Render all the game objects here

	// TODO: render your game background here

}