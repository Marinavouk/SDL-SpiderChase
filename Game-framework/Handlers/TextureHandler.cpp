#include "TextureHandler.h"

#include <SDL_image.h>
#include <iostream>

bool TextureHandler::Create(SDL_Renderer* SDLRenderer)
{
	renderer = SDLRenderer;

	return true;
}

void TextureHandler::Destroy(void)
{
	renderer = nullptr;
}

SDL_Texture* TextureHandler::CreateTexture(const std::string& fileName)
{
	SDL_Texture* texture = IMG_LoadTexture(renderer, fileName.c_str());

	if (!texture)
	{
		std::cout << "Error: failed to load texture '" << fileName << "'"  << std::endl;
		std::cout << IMG_GetError() << std::endl;
	}

	return texture;
}

SDL_Texture* TextureHandler::CreateTextureFromSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture)
	{
		std::cout << "Error: failed to create texture from surface" << std::endl;
		std::cout << SDL_GetError() << std::endl;
	}

	return texture;
}

void TextureHandler::DestroyTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(texture);
}

void TextureHandler::RenderTexture(SDL_Texture* texture, const SDL_FPoint& position, const SDL_Rect* srcRect)
{
	//int textureWidth = 0;
	//int textureHeight = 0;
	int windowWidth = 0;
	int windowHeight = 0;
	//SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
	SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

	//const SDL_FRect dstRect = {position.x, position.y, (float)textureWidth, (float)textureHeight};
	const SDL_FRect dstRect = { position.x, position.y, (float)windowWidth, (float)windowHeight };

	SDL_RenderCopyF(renderer, texture, srcRect, &dstRect);
}

