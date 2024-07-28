#include "GameState.h"

#include "Application.h"
#include "GameObjects/Chair.h"
#include "GameObjects/Player.h"
#include "GameObjects/Spider.h"
#include "GameObjects/Table.h"

#include "Handlers/AudioHandler.h"

#include <iostream>

bool CGameState::OnEnter(void)
{
#if defined(_DEBUG) 
	std::cout << "Entering game state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();
	CWindow&			window			= m_pApplication->GetWindow();

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	window.SetClearColor({150, 150, 200, 255});

	// Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)


	m_pBackground = textureHandler.CreateTexture("Assets/Textures/game_background.png");
	if (!m_pBackground)
		return false;

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/game.mp3");
	if (!m_pMusic)
		return false;

	audioHandler.PlayMusic(m_pMusic, -1);
	audioHandler.SetMusicVolume(0);

	const SDL_FPoint windowSize = window.GetSize();

	m_pPlayer = new CPlayer(m_pApplication);
	if (!m_pPlayer->Create({300.0f, windowSize.y}))
		return false;

	m_pTable = new CTable(m_pApplication);
	if (!m_pTable->Create({100.0f, windowSize.y} ))
		return false;

	m_pChair = new CChair(m_pApplication);
	if (!m_pChair->Create({900.0f, windowSize.y}))
		return false;

	m_pSpider = new CSpider(m_pApplication);
	if (!m_pSpider->Create({500.0f, windowSize.y}))
		return false;

	m_Obstacles.push_back(m_pTable);
	m_Obstacles.push_back(m_pChair);

	m_Enemies.push_back(m_pSpider);

	return true;
}

void CGameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop game music etc)

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();

	m_Enemies.clear();
	m_Obstacles.clear();

	m_pSpider->Destroy();
	delete m_pSpider;
	m_pSpider = nullptr;

	m_pChair->Destroy();
	delete m_pChair;
	m_pChair = nullptr;

	m_pTable->Destroy();
	delete m_pTable;
	m_pTable = nullptr;

	m_pPlayer->Destroy();
	delete m_pPlayer;
	m_pPlayer = nullptr;

	audioHandler.StopMusic();
	audioHandler.DestroyMusic(m_pMusic);
	m_pMusic = nullptr;

	textureHandler.DestroyTexture(m_pBackground);
	m_pBackground = nullptr;
}

void CGameState::Update(const float deltaTime)
{
	// Update all the needed game objects here

	const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

	// If the escape key on the keyboard is pressed, switch to the main menu
	if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE))
		m_pApplication->SetState(CApplication::EState::QUIT);

	m_pPlayer->HandleInput(deltaTime);
	m_pPlayer->Update(deltaTime);
	m_pPlayer->HandleCollision(m_Obstacles, m_Enemies, deltaTime);

	m_pSpider->Update(deltaTime);

	// Will fade the game music in/out whenever the game switch to/from this state
	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume((MIX_MAX_VOLUME - m_VolumeLimiter) - (int)((float)(MIX_MAX_VOLUME - m_VolumeLimiter) * transitionRenderer.GetTransitionValue()));
}

void CGameState::Render(void)
{
	// Render all the game objects here

	m_pApplication->GetTextureHandler().RenderTexture(m_pBackground, {0.0f, 0.0f}, nullptr, &m_pApplication->GetWindow().GetSize());

	m_pChair->Render();
	m_pTable->Render();
	m_pSpider->Render();
	m_pPlayer->Render();
}

void CGameState::RenderDebug(void)
{
	m_pChair->RenderDebug();
	m_pTable->RenderDebug();
	m_pSpider->RenderDebug();
	m_pPlayer->RenderDebug();
}