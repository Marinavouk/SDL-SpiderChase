#include "Pch.h"
#include "GameState.h"

#include "Application.h"
#include "GameObjects/Chair.h"
#include "GameObjects/Fireball.h"
#include "GameObjects/Player.h"
#include "GameObjects/Spider.h"
#include "GameObjects/Table.h"
#include "Globals.h"
//#include "Handlers/AudioHandler.h"
#include "Handlers/FontHandler.h"
#include "Utilities/CollisionUtilities.h"
#include "Utilities/Random.h"

bool CGameState::OnEnter(void)
{
#if defined(_DEBUG) 
	std::cout << "Entering game state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();
	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();
	const SDL_FPoint	windowSize		= m_pApplication->GetWindowSize();

	// Set the clear color (the background color that is shown behind the menu background and other objects)
	// This is completely optional
	m_pApplication->GetWindow().SetClearColor({150, 150, 200, 255});

	// Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)

	m_pBackground = textureHandler.CreateTexture("game_background.png");
	m_pBackground->SetSize(windowSize);

	m_pHeartRed = textureHandler.CreateTexture("red_heart.png");
	m_pHeartRed->SetSize({40.0f, 40.0f});

	m_pHeartBlack = textureHandler.CreateTexture("black_heart.png");
	m_pHeartBlack->SetSize({40.0f, 40.0f});

	m_pFont = fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf", 60);
	if (!m_pFont)
		return false;

	m_pCountdownFont = fontHandler.CreateFont("Assets/Fonts/SpookyWebbie-lgvxX.ttf", 150);
	if (!m_pFont)
		return false;

//	m_pMusic = audioHandler.CreateMusic("Assets/Audio/game.mp3");
//	if (!m_pMusic)
//		return false;

//	audioHandler.PlayMusic(m_pMusic, -1);
//	audioHandler.SetMusicVolume(0);

	m_pPlayer = new CPlayer(m_pApplication);
	if (!m_pPlayer->Create("player.png", {0.0f, 0.0f}, 5))
		return false;
	m_pPlayer->SetPosition({250.0f, windowSize.y - m_pPlayer->GetRectangleSize().y});
	CPlayer* player = (CPlayer*)m_pPlayer;
	player->SetAttackingCallback(	std::bind(&CGameState::OnPlayerAttacking,	this));
	player->SetDyingCallback(		std::bind(&CGameState::OnPlayerDying,		this));

	m_pTable = new CTable(m_pApplication);
	if (!m_pTable->Create("table.png", {0.0f, 0.0f}, 0 ))
		return false;
	m_pTable->SetPosition({100.0f, windowSize.y - m_pTable->GetRectangleSize().y});

	m_pChair = new CChair(m_pApplication);
	if (!m_pChair->Create("chair.png", {0.0f, 0.0f}, 0))
		return false;
	m_pChair->SetPosition({windowSize.x - (m_pChair->GetColliderSize().x + 100.0f), windowSize.y - m_pChair->GetRectangleSize().y});

	m_Obstacles.push_back(m_pTable);
	m_Obstacles.push_back(m_pChair);

	for (uint32_t i = 0; i < 10; ++i)
	{
		CGameObject* gameObject = new CSpider(m_pApplication);
		if (!gameObject->Create("spider.png", {-1000.0f, -1000.0f}, 1))
			return false;

		CSpider* spider = (CSpider*)gameObject;
		spider->SetDyingCallback(std::bind(&CGameState::OnSpiderDying, this, std::placeholders::_1));
		spider->SetTarget(m_pPlayer);
		spider->SetIndex(i);

		m_SpiderPool.push_back(gameObject);
	}

	for (uint32_t i = 0; i < 5; ++i)
	{
		SpawnSpider();
	}

	for (uint32_t i = 0; i < 10; ++i)
	{
		// Create a fireball
		CGameObject* fireball = new CFireball(m_pApplication);

		if (!fireball->Create("fire_ball.png", {-500.0f, -500.0f}, 1))
			return false;

		// Place it in the fireball pool
		m_FireballPool.push_back(fireball);
	}

	m_SpiderCount = 0;

	m_CountdownTimer	= m_CountdownTimerDefault;
	m_PreStartTimer		= m_PreStartTimerDefault;
	m_Timer				= m_TimerDefault;

	m_DeathFadeout = false;

	m_State = Estate::COUNT_DOWN;

	e_SpiderCount				= 0;
	e_EndOfRoundPlayerKilled	= false;

	return true;
}

void CGameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Easy access to handlers so you don't have to write m_pApplication->Get_X_Handler() multiple times below
	CTextureHandler&	textureHandler	= m_pApplication->GetTextureHandler();
//	CAudioHandler&		audioHandler	= m_pApplication->GetAudioHandler();
	CFontHandler&		fontHandler		= m_pApplication->GetFontHandler();

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop game music etc)

	for (CGameObject* fireball : m_FireballPool)
	{
		fireball->Destroy();
		delete fireball;
	}

	for (CGameObject* spider : m_SpiderPool)
	{
		spider->Destroy();
		delete spider;
	}

	m_ActiveFireballs.clear();
	m_ActiveSpiders.clear();
	m_SpiderPool.clear();
	m_FireballPool.clear();

	m_Obstacles.clear();

	m_pChair->Destroy();
	delete m_pChair;
	m_pChair = nullptr;

	m_pTable->Destroy();
	delete m_pTable;
	m_pTable = nullptr;

	m_pPlayer->Destroy();
	delete m_pPlayer;
	m_pPlayer = nullptr;

//	audioHandler.StopMusic();
//	audioHandler.DestroyMusic(m_pMusic);
//	m_pMusic = nullptr;

	fontHandler.DestroyFont(m_pCountdownFont);
	fontHandler.DestroyFont(m_pFont);
	m_pCountdownFont	= nullptr;
	m_pFont				= nullptr;

	textureHandler.DestroyTexture(m_pHeartBlack->GetName());
	textureHandler.DestroyTexture(m_pHeartRed->GetName());
	textureHandler.DestroyTexture(m_pBackground->GetName());
	m_pHeartBlack	= nullptr;
	m_pHeartRed		= nullptr;
	m_pBackground	= nullptr;
}

void CGameState::Update(const float deltaTime)
{
	// If the escape key on the keyboard is pressed, switch to the main menu
	if (m_pApplication->GetInputHandler().KeyPressed(SDL_SCANCODE_ESCAPE))
		m_pApplication->SetState(CApplication::EState::QUIT);

	if (m_State == Estate::IDLE)
	{
		if (!m_pApplication->GetTransitionRenderer().IsTransitioning())
			m_State = Estate::COUNT_DOWN;
	}

	else if (m_State == Estate::COUNT_DOWN)
	{
		m_pPlayer->Update(deltaTime);

		m_CountdownTimer -= deltaTime;

		if (m_CountdownTimer <= 0.0f)
		{
			m_CountdownTimer = 0.0f;

			m_State = Estate::PRE_START;
		}
	}

	else if (m_State == Estate::PRE_START)
	{
		m_pPlayer->Update(deltaTime);

		m_PreStartTimer -= deltaTime;

		if (m_PreStartTimer <= 0.0f)
		{
			m_PreStartTimer = 0.0f;

			m_State = Estate::ROUND_STARTED;
		}
	}

	else if (m_State == Estate::ROUND_STARTED)
	{
		// Update the game objects here

		m_pPlayer->HandleInput(deltaTime);
		m_pPlayer->Update(deltaTime);
		m_pPlayer->HandleObstacleCollision(m_Obstacles, deltaTime);
		m_pPlayer->HandleEnemyCollision(m_ActiveSpiders, deltaTime);

		for (CGameObject* spider : m_ActiveSpiders)
		{
			spider->Update(deltaTime);
			spider->HandleObstacleCollision(m_Obstacles, deltaTime);
		}

		for (uint32_t i = 0; i < m_ActiveFireballs.size(); ++i)
		{
			CFireball* fireball = (CFireball*)m_ActiveFireballs[i];
			fireball->Update(deltaTime);
			fireball->HandleObstacleCollision(m_Obstacles, deltaTime);

			if (fireball->GetIsDead())
			{
				m_ActiveFireballs.erase(m_ActiveFireballs.begin() + i);

				break;
			}

			bool spiderCollision = false;

			for (CGameObject* spider : m_ActiveSpiders)
			{
				if (!spider->GetIsDead() && QuadVsQuad(fireball->GetCollider(), spider->GetCollider()))
				{
					spider->Kill();
					fireball->Kill();

					m_SpiderCount++;

					m_ActiveFireballs.erase(m_ActiveFireballs.begin() + i);

					spiderCollision = true;

					break;
				}
			}

			if (spiderCollision)
				break;
		}

		m_Timer -= deltaTime;

		if(m_Timer <= 0.0f)
		{
			m_Timer = 0.0f;

			m_State = Estate::ROUND_ENDED;

			e_SpiderCount				= m_SpiderCount;
			e_EndOfRoundPlayerKilled	= false;

			m_pApplication->SetState(CApplication::EState::END_OF_ROUND);
		}
	}

	else if (m_State == Estate::ROUND_ENDED)
	{
		m_pPlayer->Update(deltaTime);
		m_pPlayer->HandleObstacleCollision(m_Obstacles, deltaTime);

		for (CGameObject* spider : m_ActiveSpiders)
		{
			spider->Update(deltaTime);
			spider->HandleObstacleCollision(m_Obstacles, deltaTime);
		}

		if (m_DeathFadeout)
		{
			m_DeathFadeDelay -= deltaTime;

			if (m_DeathFadeDelay <= 0.0f)
			{
				m_DeathFadeDelay = 0.0f;

				m_DeathFadeout = false;

				m_pApplication->SetState(CApplication::EState::END_OF_ROUND);
			}
		}
	}

	const CTransitionRenderer& transitionRenderer = m_pApplication->GetTransitionRenderer();

	// Will fade the game music in/out whenever the game switch to/from this state
//	if (transitionRenderer.IsTransitioning())
//		m_pApplication->GetAudioHandler().SetMusicVolume((MIX_MAX_VOLUME - m_VolumeLimiter) - (int)((float)(MIX_MAX_VOLUME - m_VolumeLimiter) * transitionRenderer.GetTransitionValue()));
}

void CGameState::Render(void)
{
	CFontHandler&	fontHandler = m_pApplication->GetFontHandler();
	SDL_Renderer*	renderer	= m_pApplication->GetWindow().GetRenderer();
	CWindow&		window		= m_pApplication->GetWindow();
	const SDL_FPoint windowCenter = m_pApplication->GetWindowCenter();

	// Render the game objects here

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
	m_pPlayer->Render();

	for (CGameObject* spider : m_ActiveSpiders)
	{
		spider->Render();
	}

	for (CGameObject* fireball : m_ActiveFireballs)
	{
		fireball->Render();
	}
	
	fontHandler.RenderText(renderer, m_pFont, "Time: " + std::to_string((uint32_t)ceilf(m_Timer)), {10.0f, 50.0f}, {200, 0, 0, 255});

	if(m_State == Estate::COUNT_DOWN)
	{
		const std::string	timerString = std::to_string((uint32_t)ceilf(m_CountdownTimer));
		const SDL_FPoint	textSize	= fontHandler.GetTextSize(m_pCountdownFont, timerString);

		fontHandler.RenderText(renderer, m_pCountdownFont, timerString, {windowCenter.x - (textSize.x * 0.5f), windowCenter.y - (textSize.y * 0.5f)}, {200, 0, 0, 255});
	}

	else if (m_State == Estate::PRE_START)
	{
		const std::string	preStartString	= "GO!";
		const SDL_FPoint	textSize		= fontHandler.GetTextSize(m_pCountdownFont, preStartString);

		fontHandler.RenderText(renderer, m_pCountdownFont, preStartString, {windowCenter.x - (textSize.x * 0.5f), windowCenter.y - (textSize.y * 0.5f)}, {200, 0, 0, 255});
	}
}

void CGameState::RenderDebug(void)
{
	m_pChair->RenderDebug();
	m_pTable->RenderDebug();

	for (CGameObject* spider : m_ActiveSpiders)
	{
		spider->RenderDebug();
	}

	m_pPlayer->RenderDebug();

	for (CGameObject* fireball : m_ActiveFireballs)
	{
		fireball->RenderDebug();
	}
}

void CGameState::SpawnSpider(void)
{
	CRandom*			RNG					= m_pApplication->GetRandomNumberGenerator();
	const SDL_FPoint	windowSize			= m_pApplication->GetWindowSize();
	const float			windowEdgeOffset	= 250.0f;

	// Loop through the spider pool and try to retrieve an inactive/unused spider that can be spawned on the screen
	for (CGameObject* gameObject : m_SpiderPool)
	{
		CSpider* spider = (CSpider*)gameObject;

		// Is the current spider inactive/unused?
		if (!spider->GetIsActive())
		{
			// Spawn a spider above outside the upper part of the window, in a random horizontal- and vertical position
			SDL_FPoint spawnPosition = {RNG->RandomFloat(windowEdgeOffset, (windowSize.x - spider->GetColliderSize().x) - windowEdgeOffset), RNG->RandomFloat(-windowEdgeOffset, -60)};

			while(true)
			{
				bool spiderCollision = false;

				for (CGameObject* otherSpider : m_ActiveSpiders)
				{
					const SDL_FRect otherSpiderRectangle	= otherSpider->GetRectangle();
					const SDL_FRect inactiveSpiderRectangle	= {spawnPosition.x, otherSpiderRectangle.y, spider->GetRectangle().w, spider->GetRectangle().h};

					if (QuadVsQuad(inactiveSpiderRectangle, otherSpiderRectangle))
					{
						spiderCollision = true;

						break;
					}
				}

				if (!spiderCollision)
					break;

				spawnPosition.x = RNG->RandomFloat(windowEdgeOffset, (windowSize.x - spider->GetColliderSize().x) - windowEdgeOffset);
			}

			// Select a random length the spider will hang down from the ceiling
			const float threadLength = RNG->RandomFloat(50.0f, 150.0f);
			
			spider->Activate(spawnPosition, threadLength, (uint32_t)m_ActiveSpiders.size());

			// Place the found inactive/unused spider in the m_ActiveSpiders vector
			m_ActiveSpiders.push_back(spider);

			// Should break out of the for loop now since an inactive/unused spider has been found
			break;
		}
	}
}

// This function is called whenever the player is playing its attack animation
void CGameState::OnPlayerAttacking(void)
{
	// Loop through the fireball pool and try to retrieve an inactive/unused fireball that can be spawned on the screen
	for (CGameObject* gameObject : m_FireballPool)
	{
		CFireball* fireball = (CFireball*)gameObject;

		// Is the current fireball inactive/unused?
		if (!fireball->GetIsActive())
		{
			// Inactive/unused fireball has been found

			const SDL_FPoint		playerPosition		= m_pPlayer->GetColliderPosition();
			const SDL_FPoint		playerSize			= m_pPlayer->GetColliderSize();
			const SDL_RendererFlip	playerFlipMethod	= m_pPlayer->GetFlipMethod();
			const float				horizontalOffset	= ((playerFlipMethod == SDL_RendererFlip::SDL_FLIP_NONE) ? m_pPlayer->GetColliderSize().x + 30.0f : -30.0f);

			// Spawn the fireball a bit in front of the player
			fireball->Activate({playerPosition.x + horizontalOffset, playerPosition.y + 20.0f}, playerFlipMethod);

			// Place the found inactive/unused fireball in the m_ActiveFireballs vector
			m_ActiveFireballs.push_back(fireball);

			// Should break out of the for loop now since an inactive/unused fireball has been found
			break;
		}
	}
}

// This function is called whenever the player is playing its dying animation
void CGameState::OnPlayerDying(void)
{
	m_DeathFadeDelay	= m_DeathFadeDelayDefault;
	m_DeathFadeout		= true;

	m_State = Estate::ROUND_ENDED;

	e_SpiderCount				= m_SpiderCount;
	e_EndOfRoundPlayerKilled	= true;
}

// This function is called whenever a spider is playing its dying animation
void CGameState::OnSpiderDying(const uint32_t index)
{
	m_ActiveSpiders.erase(m_ActiveSpiders.begin() + index);

	for (uint32_t i = 0; i < m_ActiveSpiders.size(); ++i)
	{
		((CSpider*)m_ActiveSpiders[i])->SetIndex(i);
	}

	SpawnSpider();
}