#include "Window.h"

#include <iostream>

bool Window::Create(const std::string& title, const SDL_Point& windowSize)
{
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN);
	if (!window)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create SDL window! " << SDL_GetError();
	#endif

		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create SDL renderer! " << SDL_GetError();
	#endif

		return false;
	}

	if (SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a) < 0)
	{
	#if defined(_DEBUG)
		std::cout << "Warning: failed to set render draw color! " << SDL_GetError();
	#endif
	}

	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0)
	{
	#if defined(_DEBUG)
		std::cout << "Warning: failed to set render draw blend mode! " << SDL_GetError();
	#endif
	}

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

	return ClearBuffer();
}

void Window::EndRender(void)
{
	SDL_RenderPresent(renderer);
}

bool Window::ClearBuffer()
{
	return  (SDL_RenderClear(renderer) == 0);
}

void Window::SetRenderTarget(SDL_Texture* renderTarget)
{
	SDL_SetRenderTarget(renderer, renderTarget);
}

void Window::OnResized()
{
	int windowWidth		= 0;
	int windowHeight	= 0;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	size = {(float)windowWidth, (float)windowHeight};
}

void Window::SetTitle(const std::string& title)
{
	SDL_SetWindowTitle(window, title.c_str());
}