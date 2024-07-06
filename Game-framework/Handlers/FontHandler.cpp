#include "FontHandler.h"

#include <iostream>

TTF_Font* FontHandler::CreateFont(const std::string& fileName, const int textSize)
{
	TTF_Font* font = TTF_OpenFont(fileName.c_str(), textSize);

	if (!font)
	{
		std::cout << "Error: failed to load font '" << fileName.c_str() << "'" << std::endl;
		std::cout << TTF_GetError() << std::endl;
	}

	return font;
}

void FontHandler::DestroyFont(TTF_Font* font)
{
	TTF_CloseFont(font);
}

bool FontHandler::RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_FPoint& position, const SDL_Color& color)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {color.r, color.g, color.b, color.a});
	if (!surface)
		return false;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	SDL_FreeSurface(surface);
	
	if (!texture)
		return false;

	int textureWidth = 0;
	int textureHeight = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

	const SDL_FRect rect = {position.x, position.y, (float)textureWidth, (float)textureHeight};

	SDL_RenderCopyF(renderer, texture, nullptr, &rect);

	SDL_DestroyTexture(texture);

	return true;
}

SDL_FPoint FontHandler::GetTextSize(TTF_Font* font, const std::string& text)
{
	int textWidth	= 0;
	int textHeight	= 0;
	TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

	return {(float)textWidth, (float)textHeight};
}