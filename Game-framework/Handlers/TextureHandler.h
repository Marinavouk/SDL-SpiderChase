#pragma once

#include <SDL.h>
#include <string>

class CTextureHandler
{
public:

	 CTextureHandler(void)						{}
	 CTextureHandler(SDL_Renderer* SDLRenderer)	{m_pRenderer = SDLRenderer;}
	~CTextureHandler(void)						{m_pRenderer = nullptr;}

	SDL_Texture*	CreateTexture(const std::string& fileName);
	SDL_Texture*	CreateTextureFromSurface(SDL_Surface* surface);
	SDL_Texture*	CreateEmptyTexture(const SDL_Point& size, const SDL_TextureAccess access);
	void			DestroyTexture(SDL_Texture* texture);

	void			RenderTexture(SDL_Texture* texture, const SDL_FPoint& position, const SDL_Rect* srcRect = nullptr, const SDL_FPoint* customSize = nullptr);
	void			RenderTextureRotated(SDL_Texture* texture, const SDL_FPoint& position, const float angle, const SDL_Rect* srcRect = nullptr, const SDL_FPoint* customSize = nullptr);

private:

	SDL_Renderer* m_pRenderer = nullptr;

};