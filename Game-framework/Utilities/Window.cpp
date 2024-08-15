#include "Pch.h"
#include "Window.h"

bool CWindow::Create(const std::string& title, const bool fullscreen, const bool resizable)
{
	SDL_Point windowSize = {1280, 720};

	SDL_DisplayMode displayMode;
	if(SDL_GetCurrentDisplayMode(0, &displayMode) == 0)
	{
		windowSize.x = (int32_t)((float)displayMode.w * 0.85f);
		windowSize.y = (int32_t)((float)displayMode.h * 0.85f);
	}

	else
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to retrieve the display mode: " << SDL_GetError() << std::endl;
		std::cout << "Reverting to default window size 1280x720" << std::endl;
	#endif
	}

	Uint32 flags = SDL_WINDOW_SHOWN;

	if (fullscreen)	flags |= SDL_WINDOW_FULLSCREEN;
	if (resizable)	flags |= SDL_WINDOW_RESIZABLE;

	m_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, flags);
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

	m_Center = {m_Size.x * 0.5f, m_Size.y * 0.5f};

	if (BeginRender())
		EndRender();

	return true;
}
void CWindow::Destroy(void)
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = nullptr;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
}

bool CWindow::BeginRender(void)
{
	return ((SDL_SetRenderDrawColor(m_pRenderer, m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a) == 0) && ClearBuffer());
}

void CWindow::EndRender(void)
{
	SDL_RenderPresent(m_pRenderer);
}

bool CWindow::ClearBuffer()
{
	return (SDL_RenderClear(m_pRenderer) == 0);
}

void CWindow::SetRenderTarget(CTexture* renderTarget)
{
	if (!renderTarget)
	{
		SDL_SetRenderTarget(m_pRenderer, nullptr);

		return;
	}

	SDL_SetRenderTarget(m_pRenderer, renderTarget->GetTexture());
}

void CWindow::OnResized()
{
	int32_t windowWidth		= 0;
	int32_t windowHeight	= 0;
	SDL_GetWindowSize(m_pWindow, &windowWidth, &windowHeight);

	m_Size = {(float)windowWidth, (float)windowHeight};

	m_Center = {m_Size.x * 0.5f, m_Size.y * 0.5f};
}

void CWindow::SetTitle(const std::string& title)
{
	SDL_SetWindowTitle(m_pWindow, title.c_str());
}