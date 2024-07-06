#include "Timer.h"

#include <SDL.h>
#include <utility>

void Timer::Update(void)
{
	currentTime		= SDL_GetTicks();
	deltaTime		= std::min((currentTime - previousTime) * 0.001, 0.25);
	previousTime	= currentTime;
}