#pragma once

// Handlers
#include "Handlers/AudioHandler.h"
#include "Handlers/FontHandler.h"
#include "Handlers/InputHandler.h"
#include "Handlers/LibraryHandler.h"
#include "Handlers/TextureHandler.h"

#include "States/State.h"
#include "States/TransitionRenderer.h"
#include "Utilities/Timer.h"
#include "Utilities/Window.h"

class Application 
{
public:

	enum EState
	{
		MAIN_MENU = 0,
		GAME,
		QUIT,
		NUM_STATES
	};

public:

	 Application(void) {}
	~Application(void) {}

	bool				Create(void); // Returns true if this application was successfully created, false otherwise
	void				Destroy(void); // Destroys all the handlers, the game etc

	// Starts the main loop
	void				Run(void);

	// Handle input events (keyboard presses, mouse press, mouse move, window X-button etc)
	void				HandleEvents(void);

	void				Update(void);
	void				Render(void);

	// This function is used to switch between states in the application/game
	// For example from the menu to the game state
	bool				SetState(const EState newState);

	void				OnTransitionOpaque(void);

public:

	// Place getters and setters in their own public field

	TextureHandler*		GetTextureHandler(void) const	{return textureHandler;}
	FontHandler*		GetFontHandler(void) const		{return fontHandler;}
	AudioHandler*		GetAudioHandler(void) const		{return audioHandler;}
	InputHandler*		GetInputHandler(void) const		{return inputHandler;}
	TransitionRenderer*	GetTransitionRenderer() const	{return transitionRenderer;}

	Window*				GetWindow(void) const			{return window;}

	// Call this to shutdown the game
	void				Quit(void)						{running = false;}

private:

	LibraryHandler*		libraryHandler		= nullptr;
	TextureHandler*		textureHandler		= nullptr;
	FontHandler*		fontHandler			= nullptr;
	AudioHandler*		audioHandler		= nullptr;
	InputHandler*		inputHandler		= nullptr;

	Window*				window				= nullptr;

	TransitionRenderer*	transitionRenderer	= nullptr;

	State*				states[NUM_STATES]	= {nullptr};
	State*				currentState		= nullptr;
	State*				nextState			= nullptr;

	Timer				timer				= {};

	bool				running				= true;

};