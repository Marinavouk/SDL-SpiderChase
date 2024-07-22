#include "TextureHandler.h"

#include <SDL_image.h>
#include <iostream>

SDL_Texture* CTextureHandler::CreateTexture(const std::string& fileName)
{
	SDL_Texture* texture = IMG_LoadTexture(m_pRenderer, fileName.c_str());

	if (!texture)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to load texture '" << fileName << "'"  << std::endl;
		std::cout << IMG_GetError() << std::endl;
	#endif
	}

	return texture;
}

SDL_Texture* CTextureHandler::CreateTextureFromSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_pRenderer, surface);

	if (!texture)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create texture from surface" << std::endl;
		std::cout << SDL_GetError() << std::endl;
	#endif
	}

	return texture;
}

SDL_Texture* CTextureHandler::CreateEmptyTexture(const SDL_Point& size, const SDL_TextureAccess access)
{
	SDL_Texture* texture = SDL_CreateTexture(m_pRenderer, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888, access, size.x, size.y);

	if (!texture)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create texture" << std::endl;
		std::cout << SDL_GetError() << std::endl;
	#endif
	}

	return texture;
}

void CTextureHandler::DestroyTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(texture);
}

void CTextureHandler::RenderTexture(SDL_Texture* texture, const SDL_FPoint& position, const SDL_Rect* srcRect, const SDL_FPoint* customSize)
{
	if (customSize)
	{
		const SDL_FRect dstRect2 = {position.x, position.y, customSize->x, customSize->y};
		SDL_RenderCopyF(m_pRenderer, texture, srcRect, &dstRect2);
	}

	else
	{
		int textureWidth = 0;
		int textureHeight = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

		const SDL_FRect dstRect2 = {position.x, position.y, (float)textureWidth, (float)textureHeight};
		SDL_RenderCopyF(m_pRenderer, texture, srcRect, &dstRect2);
	}
}

void CTextureHandler::RenderTextureRotated(SDL_Texture* texture, const SDL_FPoint& position, const float angle, const SDL_Rect* srcRect, const SDL_FPoint* customSize)
{
	if (customSize)
	{
		const SDL_FRect dstRect2 = {position.x, position.y, customSize->x, customSize->y};
		SDL_RenderCopyExF(m_pRenderer, texture, srcRect, &dstRect2, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	else
	{
		int textureWidth = 0;
		int textureHeight = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

		const SDL_FRect dstRect2 = {position.x, position.y, (float)textureWidth, (float)textureHeight};
		SDL_RenderCopyExF(m_pRenderer, texture, srcRect, &dstRect2, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}
}