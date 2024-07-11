#pragma once

#include <SDL.h>
#include <string>

class Window
{
public:

	 Window(void) {}
	~Window(void) {}

	bool			Create(const std::string& title, const SDL_Point& windowSize);
	void			Destroy(void);

	bool			BeginRender(void);
	void			EndRender(void);

	bool			ClearBuffer();

	void			SetRenderTarget(SDL_Texture* renderTarget);

	void			OnResized();

public:

	// Place getters and setters in their own public field

	SDL_Renderer*	GetRenderer(void) const					{return renderer;}

	SDL_FPoint&		GetSize(void) const						{return (SDL_FPoint&)size;}

	SDL_Color&		GetClearColor(void) const				{return (SDL_Color&)clearColor;}
	void			SetClearColor(const SDL_Color& color)	{clearColor = color;}

	void			SetTitle(const std::string& title);

private:

	SDL_Window*		window		= nullptr;
	SDL_Renderer*	renderer	= nullptr;

	SDL_FPoint		size		= {0.0f, 0.0f};

								//	R, G, B, A (alpha)
	SDL_Color		clearColor	=  {0, 0, 0, 255};

};