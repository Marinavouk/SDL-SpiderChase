#pragma once

#include "Utilities/Texture.h"

#include <SDL.h>
#include <map>
#include <string>

class CTextureHandler
{
public:

	 CTextureHandler(void) {}
	~CTextureHandler(void) {}

	bool			Create(SDL_Renderer* renderer, const std::string& basePath);
	void			Destroy(void);

	CTexture*		CreateTexture(const std::string& fileName);
	CTexture*		CreateTextureFromSurface(SDL_Surface* surface, const std::string& name);
	void			DestroyTexture(const std::string& name);

private:

	typedef std::map<std::string, CTexture*> TextureMap; 

private:

	SDL_Renderer*	m_pRenderer		= nullptr;

	std::string		m_BasePath		= "";

	TextureMap		m_TextureMap	= {};

};