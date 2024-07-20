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

	bool				Create(void);
	void				Destroy(void);

	void				Run(void);

	void				HandleEvents(void);

	void				Update(void);
	void				Render(void);

	bool				SetState(const EState newState);

	void				OnTransitionOpaque(void);

public:

	// Place getters and setters in their own public field

	TextureHandler&		GetTextureHandler(void) const	{return (TextureHandler&)m_TextureHandler;}
	FontHandler&		GetFontHandler(void) const		{return (FontHandler&)m_FontHandler;}
	AudioHandler&		GetAudioHandler(void) const		{return (AudioHandler&)m_AudioHandler;}
	InputHandler&		GetInputHandler(void) const		{return (InputHandler&)m_InputHandler;}

	Window&				GetWindow(void) const			{return (Window&)m_Window;}

	TransitionRenderer&	GetTransitionRenderer() const	{return (TransitionRenderer&)m_TransitionRenderer;}

	// Call this to shutdown the game
	void				Quit(void)						{m_Running = false;}

private:

	State*				m_pStates[NUM_STATES]	= {nullptr};
	State*				m_pCurrentState			= nullptr;
	State*				m_pNextState			= nullptr;

	Window				m_Window				= {};

	LibraryHandler		m_LibraryHandler		= {};
	TextureHandler		m_TextureHandler		= {};
	FontHandler			m_FontHandler			= {};
	AudioHandler		m_AudioHandler			= {};
	InputHandler		m_InputHandler			= {};

	TransitionRenderer	m_TransitionRenderer	= {};

	Timer				m_Timer					= {};

	bool				m_Running				= true;

};