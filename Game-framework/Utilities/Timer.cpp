#include "Timer.h"

#include <SDL.h>
#include <utility>

void CTimer::Update(void)
{
	m_CurrentTime	= SDL_GetTicks();
	m_DeltaTime		= std::min((m_CurrentTime - m_PreviousTime) * 0.001, 0.25);
	m_PreviousTime	= m_CurrentTime;
}