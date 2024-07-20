#include "Window.h"

#include <iostream>

bool Window::Create(const std::string& title, const SDL_Point& windowSize)
{
	m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN);
	if (!m_pWindow)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create SDL window! " << SDL_GetError();
	#endif

		return false;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_pRenderer)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create SDL renderer! " << SDL_GetError();
	#endif

		return false;
	}

	if (SDL_SetRenderDrawColor(m_pRenderer, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a) < 0)
	{
	#if defined(_DEBUG)
		std::cout << "Warning: failed to set render draw color! " << SDL_GetError();
	#endif
	}

	if (SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND) < 0)
	{
	#if defined(_DEBUG)
		std::cout << "Warning: failed to set render draw blend mode! " << SDL_GetError();
	#endif
	}

	m_Size = {(float)windowSize.x, (float)windowSize.y};

	if (BeginRender())
		EndRender();

	return true;
}
void Window::Destroy(void)
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = nullptr;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
}

bool Window::BeginRender(void)
{
	return ((SDL_SetRenderDrawColor(m_pRenderer, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a) == 0) && ClearBuffer());
}

void Window::EndRender(void)
{
	SDL_RenderPresent(m_pRenderer);
}

bool Window::ClearBuffer()
{
	return (SDL_RenderClear(m_pRenderer) == 0);
}

void Window::SetRenderTarget(SDL_Texture* renderTarget)
{
	SDL_SetRenderTarget(m_pRenderer, renderTarget);
}

void Window::OnResized()
{
	int windowWidth		= 0;
	int windowHeight	= 0;
	SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);

	m_Size = {(float)windowWidth, (float)windowHeight};
}

void Window::SetTitle(const std::string& title)
{
	SDL_SetWindowTitle(m_pWindow, title.c_str());
}