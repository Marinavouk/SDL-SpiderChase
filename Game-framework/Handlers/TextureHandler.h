#pragma once

#include <SDL.h>
#include <string>

class TextureHandler
{
public:

	 TextureHandler(void)						{}
	 TextureHandler(SDL_Renderer* SDLRenderer)	{renderer = SDLRenderer;}
	~TextureHandler(void)						{renderer = nullptr;}

	SDL_Texture*	CreateTexture(const std::string& fileName);
	SDL_Texture*	CreateTextureFromSurface(SDL_Surface* surface);
	SDL_Texture*	CreateEmptyTexture(const SDL_Point& size, const SDL_TextureAccess access);
	void			DestroyTexture(SDL_Texture* texture);

	void			RenderTexture(SDL_Texture* texture, const SDL_FPoint& position, const SDL_Rect* srcRect = nullptr, const SDL_FPoint* customSize = nullptr);
	void			RenderTextureRotated(SDL_Texture* texture, const SDL_FPoint& position, const float angle, const SDL_Rect* srcRect = nullptr, const SDL_FPoint* customSize = nullptr);

private:

	SDL_Renderer* renderer = nullptr;

};