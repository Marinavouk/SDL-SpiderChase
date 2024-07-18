#include "Application.h"

// The different states for the game
#include "States/GameState.h"
#include "States/MainMenuState.h"
#include "States/QuitState.h"

// Used by srand
#include <time.h>

bool Application::Create(void)
{
	// Seed the random number generator
	srand((unsigned int)time(0));

	// Creates all the libraries for you, for example SDL, SDL_Image etc
	libraryHandler = new LibraryHandler;
	if(!libraryHandler->Create())
		return false;

	window = new Window;
	if(!window->Create("Spider Chase", {1280, 720}))
		return false;

	// If you want to change the title of the window while the game is running
//	window->SetTitle("This is the window's new title");

	// If you want to set the color that the renderer's (the big 'screen texture') is "cleared" to,
	// you can use this function
//	window->SetClearColor({ 255, 0, 0, 255 });

	textureHandler		= new TextureHandler(window->GetRenderer());
	fontHandler			= new FontHandler;
	audioHandler		= new AudioHandler;
	inputHandler		= new InputHandler;
	transitionRenderer	= new TransitionRenderer(this, window->GetSize());

	// If you want to tweak the speed of the state transition, you can set the speed here
	// The lower the value is set to, the slower the transition effect will be
	// NOTE. Don't set it to 0.0f or a negative value, the transition will not work
	transitionRenderer->SetSpeed(2.0f);

	/**
	* Create the various states for the application.
	* Here you can add more states if wanted, for example a Settings menu, a Guide menu that explains the game, a Credits menu showing who made the game etc
	*/

	states[EState::MAIN_MENU]	= new MainMenuState(this);
	states[EState::GAME]		= new GameState(this);
	states[EState::QUIT]		= new QuitState(this);

	// Set the start state for the game, in this case the game will start in the MAIN_MENU state
	currentState = states[EState::MAIN_MENU];
	if(!currentState->OnEnter())
		return false;

	return true;
}

void Application::Destroy(void)
{
	if(currentState)
		currentState->OnExit();

	for(int i = 0; i < EState::NUM_STATES; ++i)
	{
		if(states[i])
		{
			delete states[i];
			states[i] = nullptr;
		}
	}

	delete transitionRenderer;
	delete inputHandler;
	delete audioHandler;
	delete fontHandler;
	delete textureHandler;
	transitionRenderer	= nullptr;
	inputHandler		= nullptr;
	audioHandler		= nullptr;
	fontHandler			= nullptr;
	textureHandler		= nullptr;

	window->Destroy();
	delete window;
	window = nullptr;

	libraryHandler->Destroy();
	delete libraryHandler;
	libraryHandler = nullptr;
}

void Application::Run(void) 
{
	// Main loop - Loops as long as the application/game is running
	while (running)
	{
		HandleEvents();
		Update();
		Render();
	}
}

void Application::HandleEvents(void)
{
	SDL_Event event = {};
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				running = false;

				break;
			}

			default:
				break;
		}
	}
}

void Application::Update(void)
{
	inputHandler->Update();
	timer.Update();

	const float deltaTime = (float)timer.GetDeltaTime();

	if(currentState)
		currentState->Update(deltaTime);

	transitionRenderer->Update(deltaTime);
}

void Application::Render(void)
{
	if(window->BeginRender())
	{
		if(currentState)
			currentState->Render();

		transitionRenderer->Render();

		window->EndRender();
	}
}

bool Application::SetState(const EState newState)
{
	// Make sure that no state transition is already happening (i.e that nextState is == nullptr)
	if(nextState)
		return false;

	nextState = states[newState];

	transitionRenderer->StartTransition();

	return true;
}

void Application::OnTransitionOpaque(void)
{
	// If there's a pending state
	if(nextState)
	{
		// Stop/exit the current state (see 'OnExit' function in each state) 
		if(currentState)
			currentState->OnExit();

		// Do the state change
		currentState = nextState;

		// And start/enter the new state (see 'OnEnter' function in each state) 
		if(!currentState->OnEnter())
			running = false;

		nextState = nullptr;
	}
}