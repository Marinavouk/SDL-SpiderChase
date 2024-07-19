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

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler* textureHandler	= application->GetTextureHandler();
	AudioHandler*	audioHandler	= application->GetAudioHandler();

	mainBackground = textureHandler->CreateTexture("Assets/Textures/game_background.png");
	if (!mainBackground)
		return false;

	table = textureHandler->CreateTexture("Assets/Textures/table.png");
	if (!table)
		return false;

	chair = textureHandler->CreateTexture("Assets/Textures/chair.png");
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
	// This is completely optional
	application->GetWindow()->SetClearColor({150, 150, 200, 255});

	return true;
}

void GameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop game music etc)

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler* textureHandler	= application->GetTextureHandler();
	AudioHandler*	audioHandler	= application->GetAudioHandler();

	audioHandler->StopMusic();
	audioHandler->DestroyMusic(music);
	music = nullptr;

	
	textureHandler->DestroyTexture(chair);
	textureHandler->DestroyTexture(table);
	textureHandler->DestroyTexture(mainBackground);
	
	chair			= nullptr;
	table			= nullptr;
	mainBackground	= nullptr;
}

void GameState::Update(const float deltaTime)
{
	// Update all the needed game objects here

	TransitionRenderer* transitionRenderer = application->GetTransitionRenderer();

	// If the escape key on the keyboard is pressed, switch to the main menu
	if (application->GetInputHandler()->KeyPressed(SDL_SCANCODE_ESCAPE))
		application->SetState(Application::EState::QUIT);

	// Will fade the game music in/out whenever the game switch to/from this state
	if (transitionRenderer->IsTransitioning())
		application->GetAudioHandler()->SetMusicVolume((MIX_MAX_VOLUME - volumeLimiter) - (int)((float)(MIX_MAX_VOLUME - volumeLimiter) * transitionRenderer->GetTransitionValue()));
}

void GameState::Render(void)
{
	// Render all the game objects here

	// It's always good practice to create a local variable for data that is used in multiple places in a function, in this the window size is used on multiple places below
	// By having a local variable like this, application->GetWindow()->GetSize() isn't called multiple times
	// This is both an optimization and also reduces repetitive code
	TextureHandler*		textureHandler	= application->GetTextureHandler();
	const SDL_FPoint	windowSize		= application->GetWindow()->GetSize();
	
	textureHandler->RenderTexture(mainBackground,	{0.0f, 0.0f},												nullptr, &windowSize);
	textureHandler->RenderTexture(table,			{200.0f, windowSize.y - tableSize.y},						nullptr, &tableSize);//need to understand here code
	textureHandler->RenderTexture(chair,			{windowSize.x - chairSize.x, windowSize.y - chairSize.y},	nullptr, &chairSize);
}