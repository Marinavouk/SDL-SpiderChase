#pragma once

class CFontHandler 
{
public:

	 CFontHandler(void) {}
	~CFontHandler(void) {}

	TTF_Font*	CreateFont(const std::string& fileName, const int32_t textSize);
	void		DestroyFont(TTF_Font* font);

	bool		RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_FPoint& position, const SDL_Color& color);

	SDL_FPoint	GetTextSize(TTF_Font* font, const std::string& text);

};