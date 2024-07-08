#pragma once

#include "State.h"

#include <SDL.h>

class GameState final : public State
{
public:

	 GameState(void)													{}
	 GameState(Application* mainApplication) : State(mainApplication)	{}
	~GameState(void)													{}

	virtual bool OnEnter(void) override;
	virtual void OnExit(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;

private:

	// Declare the game objects here and then create/destroy them in the OnEnter- and OnExit functions



};