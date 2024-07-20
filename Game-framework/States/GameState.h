#pragma once

#include "State.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include "GameObjects/Player.h"

class GameState final : public State
{
public:

	 GameState(void)											{}
	 GameState(Application* application) : State(application)	{}
	~GameState(void)											{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the game objects here and then create/destroy them in the OnEnter- and OnExit functions

	SDL_Texture*	m_pBackground	= nullptr;
	SDL_Texture*	m_pTable		= nullptr;
	SDL_Texture*	m_pChair		= nullptr;
	SDL_Texture*	m_pTriangleTest	= nullptr;

	Mix_Music*		m_pMusic		= nullptr;

	SDL_FPoint		m_TableSize		= {0.0f, 0.0f};
	SDL_FPoint		m_ChairSize		= {0.0f, 0.0f};

	Player			m_Player;

	int				m_VolumeLimiter	= 100;

};