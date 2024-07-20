#include "QuitState.h"

#include "Application.h"

bool QuitState::OnEnter(void)
{
	m_pApplication->Quit();

	return true;
}