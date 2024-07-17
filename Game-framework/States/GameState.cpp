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

	table = application->GetTextureHandler()->CreateTexture("Assets/Textures/table.png");
	if (!table)
		return false;

	chair = application->GetTextureHandler()->CreateTexture("Assets/Textures/chair.png");
	if (!chair)
		return false;

	music = audioHandler->CreateMusic("Assets/Audio/game.mp3");
	if (!music)
		return false;

	audioHandler->PlayMusic(music, -1);
	audioHandler->SetMusicVolume(0);

	int textureWidth	= 0;
	int textureHeight	= 0;
	SDL_QueryTexture(table, nullptr, nullptr, &textureWidth, &textureHeight);

	tableSize = {(float)(textureWidth * 0.7f), (float)(textureHeight * 0.7f)};

	SDL_QueryTexture(chair, nullptr, nullptr, &textureWidth, &textureHeight);

	chairSize = {(float)(textureWidth * 0.7f), (float)(textureHeight * 0.7f)};

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
		application->SetState(Application::EState::QUIT);

	if (transitionRenderer->IsTransitioning())
		application->GetAudioHandler()->SetMusicVolume((MIX_MAX_VOLUME - volumeLimiter) - (int)((float)(MIX_MAX_VOLUME - volumeLimiter) * transitionRenderer->GetTransitionValue()));
}

void GameState::Render(void)
{
	// Render all the game objects here

	const SDL_FPoint windowSize = application->GetWindow()->GetSize();

	application->GetTextureHandler()->RenderTexture(mainBackground, {0.0f, 0.0f},												nullptr, &windowSize);
	application->GetTextureHandler()->RenderTexture(table,			{200.0f, windowSize.y - tableSize.y},						nullptr, &tableSize);
	application->GetTextureHandler()->RenderTexture(chair,			{windowSize.x - chairSize.x, windowSize.y - chairSize.y},	nullptr, &chairSize);
}