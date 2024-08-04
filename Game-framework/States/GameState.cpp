#include "GameState.h"

#include "Application.h"
#include "GameObjects/Chair.h"
#include "GameObjects/Fireball.h"
#include "GameObjects/Player.h"
#include "GameObjects/Spider.h"
#include "GameObjects/Table.h"
#include "Handlers/AudioHandler.h"
#include "Utilities/Random.h"

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
	const SDL_FPoint	windowSize		= window.GetSize();

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	window.SetClearColor({150, 150, 200, 255});

	// Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)

	m_pBackground = textureHandler.CreateTexture("game_background.png");
	m_pBackground->SetSize(windowSize);

	m_pHeartRed = textureHandler.CreateTexture("red_heart.png");
	m_pHeartRed->SetSize({40.0f, 40.0f});

	m_pHeartBlack = textureHandler.CreateTexture("black_heart.png");
	m_pHeartBlack->SetSize({40.0f, 40.0f});

	m_pMusic = audioHandler.CreateMusic("Assets/Audio/game.mp3");
	if (!m_pMusic)
		return false;

	audioHandler.PlayMusic(m_pMusic, -1);
	audioHandler.SetMusicVolume(0);

	m_pPlayer = new CPlayer(m_pApplication);
	if (!m_pPlayer->Create("player.png", {150.0f, 260.0f}, 5))
		return false;
	((CPlayer*)m_pPlayer)->SetAttackCallback(std::bind(&CGameState::OnPlayerAttack, this));

	m_pTable = new CTable(m_pApplication);
	if (!m_pTable->Create("table.png", {100.0f, windowSize.y}, 0 ))
		return false;

	m_pChair = new CChair(m_pApplication);
	if (!m_pChair->Create("chair.png", {900.0f, windowSize.y}, 0))
		return false;

	m_Obstacles.push_back(m_pTable);
	m_Obstacles.push_back(m_pChair);

	m_pSpider = new CSpider(m_pApplication);
	if (!m_pSpider->Create("spider.png", {800.0f, -50.0f}, 1))
		return false;
	((CSpider*)m_pSpider)->SetTarget(m_pPlayer);

	/*
	CRandom& randomNumberGenerator = m_pApplication->GetRandomNumberGenerator();

	for (uint32_t i = 0; i < 3; ++i)
	{
		const SDL_FPoint position = {randomNumberGenerator.RandomFloat(64.0f, windowSize.x - 128.0f), randomNumberGenerator.RandomFloat(-50.0f, 0.0f)};

		CGameObject* spider = new CSpider(m_pApplication);
		if (!spider->Create("spider.png", position))
			return false;

	//	((CSpider*)spider)->SetDirection(randomNumberGenerator.RandomUint(0, 1));
		((CSpider*)spider)->SetTarget(m_pPlayer);

		m_Enemies.push_back(spider);
	}
	*/

	m_Enemies.push_back(m_pSpider);

	for (uint32_t i = 0; i < 5; ++i)
	{
		// Create a fireball
		CGameObject* fireball = new CFireball(m_pApplication);
		if (!fireball->Create("fire_ball.png", {-500.0f, -500.0f}, 1))
			return false;

		// Place it in the fireball pool
		m_FireballPool.push_back(fireball);
	}

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

	for (CGameObject* fireball : m_FireballPool)
	{
		fireball->Destroy();
		delete fireball;
	}

	m_ActiveFireballs.clear();
	m_FireballPool.clear();

	/*
	for (CGameObject* spider : m_Enemies)
	{
		spider->Destroy();
		delete spider;
		spider = nullptr;
	}
	*/

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

	textureHandler.DestroyTexture(m_pHeartBlack->GetName());
	textureHandler.DestroyTexture(m_pHeartRed->GetName());
	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pHeartBlack	= nullptr;
	m_pHeartRed		= nullptr;
	m_pBackground	= nullptr;
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
	m_pPlayer->HandleObstacleCollision(m_Obstacles, deltaTime);
	m_pPlayer->HandleEnemyCollision(m_Enemies, deltaTime);

	m_pSpider->Update(deltaTime);
	m_pSpider->HandleObstacleCollision(m_Obstacles, deltaTime);

	/*
	for (CGameObject* spider : m_Enemies)
	{
		spider->Update(deltaTime);
		spider->HandleObstacleCollision(m_Obstacles, deltaTime);
	}
	*/

	// If there's any fireball active (i.e, if the player has shot any fireballs)
	if (!m_ActiveFireballs.empty())
	{
		const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

		for (uint32_t i = 0; i < m_ActiveFireballs.size(); ++i)
		{
			CGameObject* fireball = m_ActiveFireballs[i];
			fireball->Update(deltaTime);

			const SDL_FPoint position = fireball->GetColliderPosition();

			// When the current fireball leaves the window's left- or right edge, mark it as inactive/unused again so it can be retrieved later again from m_FireballPool
			// Also remove the fireball from the m_ActiveFireballs vector so it's not updated/rendered anymore
			if ((position.x < - fireball->GetColliderSize().x) || (position.x > windowSize.x))
			{
				((CFireball*)fireball)->SetIsActive(false);

				m_ActiveFireballs.erase(m_ActiveFireballs.begin() + i);

				break;
			}
		}
	}

	// Will fade the game music in/out whenever the game switch to/from this state
	if (transitionRenderer.IsTransitioning())
		m_pApplication->GetAudioHandler().SetMusicVolume((MIX_MAX_VOLUME - m_VolumeLimiter) - (int)((float)(MIX_MAX_VOLUME - m_VolumeLimiter) * transitionRenderer.GetTransitionValue()));
}

void CGameState::Render(void)
{
	// Render all the game objects here

	m_pBackground->Render({0.0f, 0.0f});

	const SDL_FPoint	heartRedSize			= m_pHeartRed->GetSize();
	const SDL_FPoint	heartBlackSize			= m_pHeartBlack->GetSize();
	const SDL_FPoint	heartStartOffset		= {5.0f, 5.0f};
	const float			distanceBetweenHearts	= 5.0f;
	const uint32_t		playerMaxHealth			= m_pPlayer->GetMaxHealth();
	const uint32_t		playerHealth			= m_pPlayer->GetCurrentHealth();

	for (uint32_t i = 0; i < playerMaxHealth; i++)
	{
		if (i < playerHealth) 
			m_pHeartRed->Render({heartStartOffset.x + ((heartRedSize.x + distanceBetweenHearts) * i), heartStartOffset.y});

		else
			m_pHeartBlack->Render({heartStartOffset.x + ((heartBlackSize.x + distanceBetweenHearts) * i), heartStartOffset.y});
	}
	
	m_pChair->Render();
	m_pTable->Render();

	m_pSpider->Render();

	/*
	for (CGameObject* spider : m_Enemies)
	{
		spider->Render();
	}
	*/

	m_pPlayer->Render();

	// If there's any fireball active (i.e, if the player has shot any fireballs)
	if (!m_ActiveFireballs.empty())
	{
		for (CGameObject* fireball : m_ActiveFireballs)
		{
			fireball->Render();
		}
	}
}

void CGameState::RenderDebug(void)
{
	m_pChair->RenderDebug();
	m_pTable->RenderDebug();

	m_pSpider->RenderDebug();

	/*
	for (CGameObject* spider : m_Enemies)
	{
		spider->RenderDebug();
	}
	*/

	m_pPlayer->RenderDebug();
}

void CGameState::OnPlayerAttack(void)
{
#if defined(_DEBUG) 
	std::cout << "Player is now attacking and a fireball should now spawn" << std::endl;
#endif

	// This OnPlayerAttack function is called whenever the player is playing its attack animation

	const SDL_FPoint playerPosition = m_pPlayer->GetColliderPosition();
	const SDL_FPoint playerSize		= m_pPlayer->GetColliderSize();

	// Loop through the fireball pool and try to retrieve an unused fireball that can be spawned on the screen
	for (CGameObject* gameObject : m_FireballPool)
	{
		CFireball* fireball = (CFireball*)gameObject;

		// Is the current fireball inactive/unused?
		if (!fireball->GetIsActive())
		{
			// Inactive/unused fireball has been found

			// Spawn the fireball a bit in front of the player
			fireball->Activate({playerPosition.x + playerSize.x + 30.0f, playerPosition.y + 20.0f});

			// Place the found inactive/unused fireball in the m_ActiveFireballs vector
			m_ActiveFireballs.push_back(fireball);

			// Should break out of the for loop now since an inactive/unused fireball has been found
			break;
		}
	}
}