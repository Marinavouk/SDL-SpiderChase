#pragma once

// Handlers
#include "Handlers/AudioHandler.h"
#include "Handlers/FontHandler.h"
#include "Handlers/InputHandler.h"
#include "Handlers/LibraryHandler.h"
#include "Handlers/TextureHandler.h"

#include "States/State.h"
#include "States/TransitionRenderer.h"
#include "Utilities/Random.h"
#include "Utilities/Timer.h"
#include "Utilities/Window.h"

class CApplication 
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

	 CApplication(void) {}
	~CApplication(void) {}

	bool					Create(void);
	void					Destroy(void);

	void					Run(void);

	void					HandleEvents(void);

	void					Update(void);
	void					Render(void);

	bool					SetState(const EState newState);

	void					OnTransitionOpaque(void);

public:

	// Place getters and setters in their own public field

	CWindow&				GetWindow(void)	const					{return (CWindow&)m_Window;}

	CTextureHandler&		GetTextureHandler(void) const			{return (CTextureHandler&)m_TextureHandler;}
	CFontHandler&			GetFontHandler(void) const				{return (CFontHandler&)m_FontHandler;}
	CAudioHandler&			GetAudioHandler(void) const				{return (CAudioHandler&)m_AudioHandler;}
	CInputHandler&			GetInputHandler(void) const				{return (CInputHandler&)m_InputHandler;}

	CTransitionRenderer&	GetTransitionRenderer(void) const		{return (CTransitionRenderer&)m_TransitionRenderer;}

	CRandom&				GetRandomNumberGenerator(void) const	{return (CRandom&)m_RandomNumberGenerator;}

	// Call this to shutdown the game
	void					Quit(void)								{m_Running = false;}

private:

	CState*				m_pStates[NUM_STATES]	= {nullptr};
	CState*				m_pCurrentState			= nullptr;
	CState*				m_pNextState			= nullptr;

	CWindow				m_Window				= {};

	CLibraryHandler		m_LibraryHandler		= {};
	CTextureHandler		m_TextureHandler		= {};
	CFontHandler		m_FontHandler			= {};
	CAudioHandler		m_AudioHandler			= {};
	CInputHandler		m_InputHandler			= {};

	CTransitionRenderer	m_TransitionRenderer	= {};

	CTimer				m_Timer					= {};

	CRandom				m_RandomNumberGenerator	= {};

	bool				m_Running				= true;
	bool				m_DebugRendering		= false;

};