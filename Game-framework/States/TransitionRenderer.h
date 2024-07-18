#pragma once

#include <SDL.h>

class Application;

class TransitionRenderer
{
public:

	 TransitionRenderer(void);
	 TransitionRenderer(Application* mainApplication, const SDL_FPoint& size);
	~TransitionRenderer(void);

	void	Update(const float deltaTime);
	void	Render(void);

	void	StartTransition(void);

public:

	// Place getters and setters in their own public field

	void	SetColor(const SDL_Color& transitionColor)	{color = transitionColor;}

	float	GetTransitionValue(void) const				{return transitionValue;}

	void	SetSpeed(const float transitionSpeed)		{fadeSpeed = transitionSpeed;}

	bool	IsTransitioning(void) const					{return (state != EState::IDLE);}

private:

	enum EState
	{
		IDLE = 0,
		FADING_IN,
		FADING_OUT
	};

private:

	Application*	application				= nullptr;

	SDL_Renderer*	renderer				= nullptr;

	SDL_FRect		rect					= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_Color		color					= {0, 0, 0, 255};

	float			transitionValue			= 1.0f;
	float			transitionDelayDefault	= 0.3f;
	float			transitionDelay			= transitionDelayDefault;
	float			fadeSpeed				= 1.0f;

	EState			state					= EState::FADING_IN;

};