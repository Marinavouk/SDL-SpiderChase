#include "QuitState.h"

#include "Application.h"

bool QuitState::OnEnter(void)
{
	application->Quit();

	return true;
}