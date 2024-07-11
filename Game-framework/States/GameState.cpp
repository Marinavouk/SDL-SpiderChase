#include "GameState.h"

#include "Application.h"
#include "Handlers/AudioHandler.h"

#include <iostream>

bool GameState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering game state" << std::endl;
#endif

	// Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)

	mainBackground = application->GetTextureHandler()->CreateTexture("Assets/Textures/game_background.png");
	if (!mainBackground)
		return false;

	music = application->GetAudioHandler()->CreateMusic("Assets/Audio/gameMusic.mp3");
	if (!music)
		return false;

	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * application->GetTransitionRenderer()->GetTransitionValue()));

	// Set the clear color (the background color that is shown behind the menu background and other objects)
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

	Mix_HaltMusic();
	application->GetAudioHandler()->DestroyMusic(music);
	music = nullptr;

	application->GetTextureHandler()->DestroyTexture(mainBackground);
	mainBackground = nullptr;
}

void GameState::Update(const float deltaTime)
{
	// Update all the needed game objects here

	TransitionRenderer* transitionRenderer = application->GetTransitionRenderer();

	// If the escape key on the keyboard is pressed, switch to the main menu
	if (application->GetInputHandler()->KeyPressed(SDL_SCANCODE_ESCAPE))
		application->SetState(Application::EState::MAIN_MENU);

	if (transitionRenderer->IsTransitioning())
		Mix_VolumeMusic(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer->GetTransitionValue()));
}

void GameState::Render(void)
{
	// Render all the game objects here

	const SDL_FPoint	windowSize	= application->GetWindow()->GetSize();
	const SDL_FRect		dstRect		= {0.0f, 0.0f, windowSize.x, windowSize.y}; 

	application->GetTextureHandler()->RenderTexture(mainBackground, {0.0f, 0.0f}, nullptr, &dstRect);
}