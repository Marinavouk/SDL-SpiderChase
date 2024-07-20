#pragma once

#include <SDL.h>

class Application;

class TransitionRenderer
{
public:

	 TransitionRenderer(void);
	 TransitionRenderer(Application* application, const SDL_FPoint& size);
	~TransitionRenderer(void);

	void	Update(const float deltaTime);
	void	Render(void);

	void	StartTransition(void);

public:

	// Place getters and setters in their own public field

	void	SetColor(const SDL_Color& color)	{m_Color = color;}

	float	GetTransitionValue(void) const		{return m_TransitionValue;}

	void	SetSpeed(const float speed)			{m_FadeSpeed = speed;}

	bool	IsTransitioning(void) const			{return (m_State != EState::IDLE);}

private:

	enum EState
	{
		IDLE = 0,
		FADING_IN,
		FADING_OUT
	};

private:

	Application*	m_pApplication				= nullptr;

	SDL_Renderer*	m_pRenderer					= nullptr;

	SDL_FRect		m_Transition				= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_Color		m_Color						= {0, 0, 0, 255};

	float			m_TransitionValue			= 1.0f;
	float			m_TransitionDelayDefault	= 0.3f;
	float			m_TransitionDelay			= m_TransitionDelayDefault;
	float			m_FadeSpeed					= 1.0f;

	EState			m_State						= EState::FADING_IN;

};