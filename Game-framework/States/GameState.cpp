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

	// Easy access to the audio handler so you don't have to write application->GetAudioHandler() multiple times below
	AudioHandler* audioHandler = application->GetAudioHandler();

	mainBackground = application->GetTextureHandler()->CreateTexture("Assets/Textures/game_background.png");
	if (!mainBackground)
		return false;

	table = application->GetTextureHandler()->CreateTexture("Assets/Textures/tableSpiderChase.png");
	if (!table)
		return false;

	chair = application->GetTextureHandler()->CreateTexture("Assets/Textures/chairSpiderChase.png");
	if (!chair)
		return false;

	music = audioHandler->CreateMusic("Assets/Audio/game.mp3");
	if (!music)
		return false;

	audioHandler->PlayMusic(music, -1);
	audioHandler->SetMusicVolume(0);

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

	// Easy access to the audio handler so you don't have to write application->GetAudioHandler() multiple times below
	AudioHandler* audioHandler = application->GetAudioHandler();

	audioHandler->StopMusic();
	audioHandler->DestroyMusic(music);
	music = nullptr;

	application->GetTextureHandler()->DestroyTexture(chair);
	chair = nullptr;

	application->GetTextureHandler()->DestroyTexture(table);
	table = nullptr;

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
		application->GetAudioHandler()->SetMusicVolume((MIX_MAX_VOLUME - volumeLimiter) - (int)((float)(MIX_MAX_VOLUME - volumeLimiter) * transitionRenderer->GetTransitionValue()));
}

void GameState::Render(void)
{
	// Render all the game objects here

	const SDL_FPoint	windowSize	= application->GetWindow()->GetSize();
	const SDL_FRect		dstRect		= {0.0f, 0.0f, windowSize.x, windowSize.y}; 

	application->GetTextureHandler()->RenderTexture(mainBackground, {0.0f, 0.0f}, nullptr, &dstRect);
	application->GetTextureHandler()->RenderTexture(table, {0.0f, 400.0f}, nullptr, nullptr);
	application->GetTextureHandler()->RenderTexture(chair, {960.0f, 400.0f}, nullptr, nullptr);
}