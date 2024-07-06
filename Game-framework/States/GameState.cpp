#include "GameState.h"

#include "Application.h"
#include "Handlers/InputHandler.h"

#include <iostream>

bool GameState::Create(Application* mainApplication)
{
#if defined(_DEBUG)
	std::cout << "Creating game state" << std::endl;
#endif

	State::Create(mainApplication);

	// Create objects here that should be created once and that should persist during the lifetime of the game



	return true;
}

void GameState::Destroy(void)
{
#if defined(_DEBUG)
	std::cout << "Destroying game state" << std::endl;
#endif

	// Destroy objects here that has been created in the Create function and should be destroyed in the end of the game's lifetime



	State::Destroy();
}

bool GameState::OnEnter(void)
{
#if defined(_DEBUG)
	std::cout << "Entering game state" << std::endl;
#endif

	// Create objects that should be created/started when this state is entered/started (create textures, load/start game music etc)

	const SDL_FPoint	windowSize				= application->GetWindow()->GetSize();
	const SDL_FPoint	windowSizeHalf			= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_Color		buttonBackgroundColor	= {100, 100, 100, 150}; // Gray
	const SDL_Color		buttonTextColor			= {255, 255, 255, 255}; // White
	const SDL_Color		buttonTextHoveredColor	= {255,	  0,   0, 255}; // Red

	buttonFont = application->GetFontHandler()->CreateFont("Assets/Fonts/RockwellNova-Light.ttf", 50);
	if (!buttonFont)
		return false;

	texture = application->GetTextureHandler()->CreateTexture("Assets/Textures/Character.png");
	if (!texture)
		return false;

	menuButton = new Button;
	if (!menuButton->Create(application, buttonFont, "Menu", {120.0f, 0.0f}, buttonBackgroundColor, buttonTextColor, buttonTextHoveredColor))
		return false;
	menuButton->SetPosition({windowSizeHalf.x, windowSizeHalf.y + 150.0f});

	music = application->GetAudioHandler()->CreateMusic("Assets/Audio/game.mp3");
	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * application->GetTransitionRenderer()->GetTransitionValue()));

	return true;
}

void GameState::OnExit(void)
{
#if defined(_DEBUG)
	std::cout << "Exiting game state" << std::endl;
#endif

	// Destroy objects that should be destroyed/stopped when this state is exited/stopped (destroy textures, unload/stop game music etc)

	Mix_HaltMusic();
	application->GetAudioHandler()->DestroyMusic(music);
	music = nullptr;

	menuButton->Destroy();
	delete menuButton;
	menuButton = nullptr;

	application->GetTextureHandler()->DestroyTexture(texture);
	texture = nullptr;

	application->GetFontHandler()->DestroyFont(buttonFont);
	buttonFont = nullptr;
}

void GameState::Update(const float deltaTime)
{
	// Update all the needed game objects here

	InputHandler* inputHandler = application->GetInputHandler();

	if (menuButton->PointInside(inputHandler->GetMousePosition()) && inputHandler->MouseButtonPressed(SDL_BUTTON_LEFT))
		application->SetState(Application::EState::MAIN_MENU);

	else if(inputHandler->KeyPressed(SDL_SCANCODE_ESCAPE))
		application->SetState(Application::EState::MAIN_MENU);

	TransitionRenderer* transitionRenderer = application->GetTransitionRenderer();

	if (transitionRenderer->IsTransitioning())
		Mix_VolumeMusic(MIX_MAX_VOLUME - (int)((float)MIX_MAX_VOLUME * transitionRenderer->GetTransitionValue()));
}

void GameState::Render(void)
{
	// Render all the game objects here

	SDL_Renderer*		renderer		= application->GetWindow()->GetRenderer();
	FontHandler*		fontHandler		= application->GetFontHandler();
	const std::string	message			= "You're in the game state";
	const SDL_FPoint	windowSize		= application->GetWindow()->GetSize();
	const SDL_FPoint	windowSizeHalf	= {windowSize.x * 0.5f, windowSize.y * 0.5f};
	const SDL_FPoint	textSize		= fontHandler->GetTextSize(buttonFont, message);

	application->GetTextureHandler()->RenderTexture(texture, {100.0f, 100.0f});

	fontHandler->RenderText(renderer, buttonFont, message, {windowSizeHalf.x - (textSize.x * 0.5f), windowSizeHalf.y - ((textSize.y * 0.5f) + 50.0f)}, {255, 255, 255, 255});

	menuButton->Render(renderer, application->GetInputHandler()->GetMousePosition());
}
//need to delete stuff here