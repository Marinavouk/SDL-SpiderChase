#pragma once

#include <SDL.h>
#include <string>

class TextureHandler
{
public:

	 TextureHandler(void) {}
	~TextureHandler(void) {}

	bool			Create(SDL_Renderer* SDLRenderer);
	void			Destroy(void);

	SDL_Texture*	CreateTexture(const std::string& fileName);
	SDL_Texture*	CreateTextureFromSurface(SDL_Surface* surface);
	void			DestroyTexture(SDL_Texture* texture);

	void			RenderTexture(SDL_Texture* texture, const SDL_FPoint& position, const SDL_Rect* srcRect = nullptr, const SDL_FRect* dstRect = nullptr);
	void			RenderTextureRotated(SDL_Texture* texture, const SDL_FPoint& position, const float angle, const SDL_Rect* srcRect = nullptr, const SDL_FRect* dstRect = nullptr);

private:

	SDL_Renderer* renderer = nullptr;

};