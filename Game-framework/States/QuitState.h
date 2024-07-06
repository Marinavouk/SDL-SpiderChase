#pragma once

#include "State.h"

class QuitState final : public State
{
public:

	 QuitState(void) {}
	~QuitState(void) {}

	virtual bool OnEnter(void) override;

};