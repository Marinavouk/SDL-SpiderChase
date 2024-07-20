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
	TextureHandler& textureHandler	= m_pApplication->GetTextureHandler();
	AudioHandler&	audioHandler	= m_pApplication->GetAudioHandler();

	m_pBackground = textureHandler.CreateTexture("Assets/Textures/game_background.png");
	if (!m_pBackground)
		return false;

	m_pTable = textureHandler.CreateTexture("Assets/Textures/table.png");
	if (!m_pTable)
		return false;

	m_pChair = textureHandler.CreateTexture("Assets/Textures/chair.png");
	if (!m_pChair)
		return false;

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/game.mp3");
	if (!m_pMusic)
		return false;

	audioHandler.PlayMusic(m_pMusic, -1);
	audioHandler.SetMusicVolume(0);

	int textureWidth	= 0;
	int textureHeight	= 0;
	SDL_QueryTexture(m_pTable, nullptr, nullptr, &textureWidth, &textureHeight);

	m_TableSize = {(float)(textureWidth * 0.7f), (float)(textureHeight * 0.7f)};

	SDL_QueryTexture(m_pChair, nullptr, nullptr, &textureWidth, &textureHeight);

	m_ChairSize = {(float)(textureWidth * 0.7f), (float)(textureHeight * 0.7f)};

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	m_pApplication->GetWindow().SetClearColor({150, 150, 200, 255});

	return true;
}

void GameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop game music etc)

	// Easy access to handlers so you don't have to write application->Get_X_Handler() multiple times below
	TextureHandler& textureHandler	= m_pApplication->GetTextureHandler();
	AudioHandler&	audioHandler	= m_pApplication->GetAudioHandler();

	audioHandler.StopMusic();
	audioHandler.DestroyMusic(m_pMusic);
	m_pMusic = nullptr;

	textureHandler.DestroyTexture(m_pChair);
	textureHandler.DestroyTexture(m_pTable);
	textureHandler.DestroyTexture(m_pBackground);
	m_pChair			= nullptr;
	m_pTable			= nullptr;
	m_pBackground	= nullptr;
}

void GameState::Update(const float deltaTime)
{
	// Update all the needed game objects here

	const TransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

	// If the escape key on the keyboard is pressed, switch to the main menu
	if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE))
		m_pApplication->SetState(Application::EState::QUIT);

	// Will fade the game music in/out whenever the game switch to/from this state
	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume((MIX_MAX_VOLUME - m_VolumeLimiter) - (int)((float)(MIX_MAX_VOLUME - m_VolumeLimiter) * transitionRenderer.GetTransitionValue()));
}

void GameState::Render(void)
{
	// Render all the game objects here

	// It's always good practice to create a local variable for data that is used in multiple places in a function, in this the window size is used on multiple places below
	// By having a local variable like this, application->GetWindow()->GetSize() isn't called multiple times
	// This is both an optimization and also reduces repetitive code
	TextureHandler&		textureHandler	= m_pApplication->GetTextureHandler();
	const SDL_FPoint	windowSize		= m_pApplication->GetWindow().GetSize();
	
	textureHandler.RenderTexture(m_pBackground,	{0.0f, 0.0f},												nullptr, &windowSize);
	textureHandler.RenderTexture(m_pTable,				{200.0f, windowSize.y - m_TableSize.y},						nullptr, &m_TableSize);//need to understand here code
	textureHandler.RenderTexture(m_pChair,				{windowSize.x - m_ChairSize.x, windowSize.y - m_ChairSize.y},	nullptr, &m_ChairSize);
}