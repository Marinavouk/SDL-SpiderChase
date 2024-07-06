#include "Window.h"

#include <iostream>

bool Window::Create(const std::string& title, const SDL_Point& windowSize)
{
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cout << "Error: failed to create SDL window! " << SDL_GetError();

		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		std::cout << "Error: failed to create SDL renderer! " << SDL_GetError();

		return false;
	}

	if (SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a) < 0)
		std::cout << "Warning: failed to set render draw color! " << SDL_GetError();

	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0)
		std::cout << "Warning: failed to set render draw blend mode! " << SDL_GetError();

	size = {(float)windowSize.x, (float)windowSize.y};

	if (BeginRender())
		EndRender();

	return true;
}
void Window::Destroy(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

bool Window::BeginRender(void)
{
	if (SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a) < 0)
		return false;

	if (SDL_RenderClear(renderer) < 0)
		return false;

	return true;
}

void Window::EndRender(void)
{
	SDL_RenderPresent(renderer);
}

void Window::SetTitle(const std::string& title)
{
	SDL_SetWindowTitle(window, title.c_str());
}