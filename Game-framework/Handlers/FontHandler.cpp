#include "Pch.h"
#include "FontHandler.h"

TTF_Font* CFontHandler::CreateFont(const std::string& fileName, const int32_t textSize)
{
	TTF_Font* font = TTF_OpenFont(fileName.c_str(), textSize);

	if (!font)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to load font '" << fileName.c_str() << "'" << std::endl;
		std::cout << TTF_GetError() << std::endl;
	#endif
	}

	return font;
}

void CFontHandler::DestroyFont(TTF_Font* font)
{
	TTF_CloseFont(font);
}

bool CFontHandler::RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_FPoint& position, const SDL_Color& color)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {color.r, color.g, color.b, color.a});
	if (!surface)
		return false;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	SDL_FreeSurface(surface);
	
	if (!texture)
		return false;

	int32_t textureWidth = 0;
	int32_t textureHeight = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

	const SDL_FRect rect = {position.x, position.y, (float)textureWidth, (float)textureHeight};

	SDL_RenderCopyF(renderer, texture, nullptr, &rect);

	SDL_DestroyTexture(texture);

	return true;
}

SDL_FPoint CFontHandler::GetTextSize(TTF_Font* font, const std::string& text)
{
	int32_t textWidth	= 0;
	int32_t textHeight	= 0;
	TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

	return {(float)textWidth, (float)textHeight};
}