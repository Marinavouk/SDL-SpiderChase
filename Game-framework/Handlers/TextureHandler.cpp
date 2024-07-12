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
	#if defined(_DEBUG)
		std::cout << "Error: failed to load texture '" << fileName << "'"  << std::endl;
		std::cout << IMG_GetError() << std::endl;
	#endif
	}

	return texture;
}

SDL_Texture* TextureHandler::CreateTextureFromSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create texture from surface" << std::endl;
		std::cout << SDL_GetError() << std::endl;
	#endif
	}

	return texture;
}

SDL_Texture* TextureHandler::CreateEmptyTexture(const SDL_Point& size, const SDL_TextureAccess access)
{
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888, access, size.x, size.y);

	if (!texture)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create texture" << std::endl;
		std::cout << SDL_GetError() << std::endl;
	#endif
	}

	return texture;
}

void TextureHandler::DestroyTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(texture);
}

void TextureHandler::RenderTexture(SDL_Texture* texture, const SDL_FPoint& position, const SDL_Rect* srcRect, const SDL_FRect* dstRect)
{
	if (dstRect)
	{
		const SDL_FRect dstRect2 = {position.x, position.y, dstRect->w, dstRect->h};
		SDL_RenderCopyF(renderer, texture, srcRect, &dstRect2);
	}

	else
	{
		int textureWidth = 0;
		int textureHeight = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

		const SDL_FRect dstRect2 = {position.x, position.y, (float)textureWidth, (float)textureHeight};
		SDL_RenderCopyF(renderer, texture, srcRect, &dstRect2);
	}
}

void TextureHandler::RenderTextureRotated(SDL_Texture* texture, const SDL_FPoint& position, const float angle, const SDL_Rect* srcRect /*= nullptr*/, const SDL_FRect* dstRect /*= nullptr*/)
{
	if (dstRect)
	{
		const SDL_FRect dstRect2 = {position.x, position.y, dstRect->w, dstRect->h};
		SDL_RenderCopyExF(renderer, texture, srcRect, &dstRect2, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	else
	{
		int textureWidth = 0;
		int textureHeight = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

		const SDL_FRect dstRect2 = {position.x, position.y, (float)textureWidth, (float)textureHeight};
		SDL_RenderCopyExF(renderer, texture, srcRect, &dstRect2, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}
}