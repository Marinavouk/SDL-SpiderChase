#include "MainMenuState.h"

#include "Application.h"
#include "Handlers/AudioHandler.h"
#include "Handlers/InputHandler.h"

#include <iostream>

bool MainMenuState::Create(Application* mainApplication)
{
#if defined(_DEBUG)
	std::cout << "Creating menu state" << std::endl;
#endif

	State::Create(mainApplication);

	// Create objects here that should be created once and that should persist during the lifetime of the game

	menyBackground = application->GetTextureHandler()->CreateTexture("Assets/Textures/mainmenySpiderChase.jpg");


	return true;
}

void MainMenuState::Destroy(void)
{
#if defined(_DEBUG)
	std::cout << "Destroying menu state" << std::endl;
#endif

	// Destroy objects here that has been created in the Create function and should be destroyed in the end of the game's lifetime
	application->GetTextureHandler()->DestroyTexture(menyBackground);


	State::Destroy();
}

bool MainMenuState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering menu state" << std::endl;
#endif

	// Create objects that should be created/started when this state is entered/started (create textures and buttons, load/start main menu music etc)

	const SDL_FPoint	windowSize				= application->GetWindow()->GetSize();
	const SDL_FPoint	windowSizeHalf			= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_Color		buttonBackgroundColor	= { 50,	 50,  50, 150};	// Dark gray
	const SDL_Color		buttonTextColor			= {255, 255, 255, 255}; // White
	const SDL_Color		buttonTextHoveredColor	= {255,	  0,   0, 255}; // Red

	buttonFont = application->GetFontHandler()->CreateFont("Assets/Fonts/RockwellNova-Light.ttf", 50);
	if (!buttonFont)
		return false;

	playButton = new Button;
	quitButton = new Button;
	if (!playButton->Create(application, buttonFont, "Play", {120.0f, 0.0f}, buttonBackgroundColor, buttonTextColor, buttonTextHoveredColor)) return false;
	if (!quitButton->Create(application, buttonFont, "Quit", {120.0f, 0.0f}, buttonBackgroundColor, buttonTextColor, buttonTextHoveredColor)) return false;
	playButton->SetPosition({windowSizeHalf.x, windowSizeHalf.y + 150.0f});
	quitButton->SetPosition({windowSizeHalf.x, windowSizeHalf.y + 220.0f});

	const float value = application->GetTransitionRenderer()->GetTransitionValue();

	music = application->GetAudioHandler()->CreateMusic("Assets/Audio/menu.mp3");
	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * application->GetTransitionRenderer()->GetTransitionValue()));

	return true;
}

void MainMenuState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting menu state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures and buttons, unload/stop main menu music etc)

	Mix_HaltMusic();
	application->GetAudioHandler()->DestroyMusic(music);
	music = nullptr;

	quitButton->Destroy();
	playButton->Destroy();
	delete quitButton;
	delete playButton;
	playButton = nullptr;
	quitButton = nullptr;

	application->GetFontHandler()->DestroyFont(buttonFont);
}

void MainMenuState::Update(const float deltaTime)
{
	// Update all the needed main menu objects here

	InputHandler*		inputHandler	= application->GetInputHandler();
	const SDL_FPoint	mousePosition	= inputHandler->GetMousePosition();

	if (playButton->PointInside(mousePosition) && inputHandler->MouseButtonPressed(SDL_BUTTON_LEFT))
		application->SetState(Application::EState::GAME);

	else if (quitButton->PointInside(mousePosition) && inputHandler->MouseButtonPressed(SDL_BUTTON_LEFT))
		application->SetState(Application::EState::QUIT);

	if (inputHandler->KeyPressed(SDL_SCANCODE_RETURN))
		application->SetState(Application::EState::GAME);

	else if (inputHandler->KeyPressed(SDL_SCANCODE_ESCAPE))
		application->SetState(Application::EState::QUIT);

	TransitionRenderer* transitionRenderer = application->GetTransitionRenderer();

	if (transitionRenderer->IsTransitioning())
		Mix_VolumeMusic(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer->GetTransitionValue()));
}

void MainMenuState::Render(void)
{
	// Render all the main menu objects here

	SDL_Renderer*		renderer		= application->GetWindow()->GetRenderer();
	FontHandler*		fontHandler		= application->GetFontHandler();
	
	const SDL_FPoint	mousePosition	= application->GetInputHandler()->GetMousePosition();
	const std::string	message			= "You're in the menu state";
	const SDL_FPoint	windowSize		= application->GetWindow()->GetSize();
	const SDL_FPoint	windowSizeHalf	= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_FPoint	textSize		= fontHandler->GetTextSize(buttonFont, message);

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, nullptr);

	application->GetTextureHandler()->RenderTexture(menyBackground, { 0.0f, 0.0f});
	fontHandler->RenderText(renderer, buttonFont, message, {windowSizeHalf.x - (textSize.x * 0.5f), windowSizeHalf.y - ((textSize.y * 0.5f) + 50.0f)}, {255, 255, 255, 255});

	playButton->Render(renderer, mousePosition);
	quitButton->Render(renderer, mousePosition);
}