#include "Application.h"

// The different states for the game
#include "States/GameState.h"
#include "States/MainMenuState.h"
#include "States/QuitState.h"

// Used by srand
#include <time.h>

bool CApplication::Create(void)
{
	// Seed the random number generator
	srand((unsigned int)time(0));

	// Creates all the libraries for you, for example SDL, SDL_Image etc
	if(!m_LibraryHandler.Create())
		return false;

	if(!m_Window.Create("Spider Chase", {1280, 720}))
		return false;

	// If you want to change the title of the window while the game is running
//	m_Window.SetTitle("This is the window's new title");

	// If you want to set the color that the renderer's (the big 'screen texture') is "cleared" to,
	// you can use this function
//	m_Window.SetClearColor({ 255, 0, 0, 255 });

	m_TextureHandler = CTextureHandler(m_Window.GetRenderer());

	m_TransitionRenderer = CTransitionRenderer(this, m_Window.GetSize());

	// If you want to tweak the speed of the state transition, you can set the speed here
	// The lower the value is set to, the slower the transition effect will be
	// NOTE. Don't set it to 0.0f or a negative value, the transition will not work
	m_TransitionRenderer.SetSpeed(100.0f); // Default: 2.0f

	/**
	* Create the various states for the application.
	* Here you can add more states if wanted, for example a Settings menu, a Guide menu that explains the game, a Credits menu showing who made the game etc
	*/

	m_pStates[EState::MAIN_MENU]	= new CMainMenuState(this);
	m_pStates[EState::GAME]			= new CGameState(this);
	m_pStates[EState::QUIT]			= new CQuitState(this);

	// Set the start state for the game, in this case the game will start in the MAIN_MENU state
	m_pCurrentState = m_pStates[EState::GAME];
	if(!m_pCurrentState->OnEnter())
		return false;

	return true;
}

void CApplication::Destroy(void)
{
	if(m_pCurrentState)
		m_pCurrentState->OnExit();

	for(int i = 0; i < EState::NUM_STATES; ++i)
	{
		if(m_pStates[i])
		{
			delete m_pStates[i];
			m_pStates[i] = nullptr;
		}
	}

	m_Window.Destroy();
	m_LibraryHandler.Destroy();
}

void CApplication::Run(void) 
{
	// Main loop - Loops as long as the application/game is running
	while (m_Running)
	{
		HandleEvents();
		Update();
		Render();
	}
}

void CApplication::HandleEvents(void)
{
	SDL_Event event = {};
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				m_Running = false;

				break;
			}

			default:
				break;
		}
	}
}

void CApplication::Update(void)
{
	m_InputHandler.Update();
	m_Timer.Update();

	const float deltaTime = (float)m_Timer.GetDeltaTime();

	if(m_pCurrentState)
		m_pCurrentState->Update(deltaTime);

	m_TransitionRenderer.Update(deltaTime);
}

void CApplication::Render(void)
{
	if(m_Window.BeginRender())
	{
		if(m_pCurrentState)
			m_pCurrentState->Render();

		if(m_pCurrentState)
		m_pCurrentState->RenderDebug();

		m_TransitionRenderer.Render();

		m_Window.EndRender();
	}
}

bool CApplication::SetState(const EState newState)
{
	// Make sure that no state transition is already happening, i.e make sure that m_pNextState is nullptr
	// If m_pNextState is not nullptr, it means a state transition is occurring
	if(m_pNextState)
		return false;

	m_pNextState = m_pStates[newState];

	m_TransitionRenderer.StartTransition();

	return true;
}

void CApplication::OnTransitionOpaque(void)
{
	// If there's a pending state
	if(m_pNextState)
	{
		// Stop/exit the current state (see 'OnExit' function in each state) 
		if(m_pCurrentState)
			m_pCurrentState->OnExit();

		// Do the state change
		m_pCurrentState = m_pNextState;

		// And start/enter the new state (see 'OnEnter' function in each state) 
		if(!m_pCurrentState->OnEnter())
			m_Running = false;

		m_pNextState = nullptr;
	}
}