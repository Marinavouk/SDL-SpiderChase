#pragma once

#include "State.h"

class QuitState final : public State
{
public:

	 QuitState(void)											{}
	 QuitState(Application* application) : State(application)	{}
	~QuitState(void)											{}

	virtual bool OnEnter(void) override;

};