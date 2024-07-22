#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class CFontHandler 
{
public:

	 CFontHandler(void) {}
	~CFontHandler(void) {}

	TTF_Font*	CreateFont(const std::string& fileName, const int textSize);
	void		DestroyFont(TTF_Font* font);

	bool		RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_FPoint& position, const SDL_Color& color);

	SDL_FPoint	GetTextSize(TTF_Font* font, const std::string& text);

};