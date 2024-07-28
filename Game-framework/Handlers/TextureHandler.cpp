#include "TextureHandler.h"

#include "Utilities/StringUtilities.h"

#include <SDL_image.h>
#include <iostream>

bool CTextureHandler::Create(SDL_Renderer* renderer, const std::string& basePath)
{
	m_pRenderer = renderer;

	m_BasePath = basePath;

	CTexture* defaultTexture = CreateTexture("default.png");

	if(!defaultTexture)
		return false;

	m_TextureMap["default"] = defaultTexture;

	return true;
}

void CTextureHandler::Destroy(void)
{
	DestroyTexture("default");

	m_TextureMap.clear();

	m_pRenderer = nullptr;
}

CTexture* CTextureHandler::CreateTexture(const std::string& fileName)
{
	if(fileName.empty())
		m_TextureMap.find("default")->second;

	const std::string name = RemoveFileExtension(fileName);

	TextureMap::const_iterator FindIt = m_TextureMap.find(name);

	if(FindIt != m_TextureMap.end())
	{
		FindIt->second->m_ReferenceCount++;

		return FindIt->second;
	}

	CTexture* texture = new CTexture;

	if(!texture->Create(m_pRenderer, m_BasePath + "/" + fileName))
	{
		delete texture;
		texture = nullptr;

		return (!m_TextureMap.empty() ? m_TextureMap.find("default")->second : nullptr); 
	}

	texture->m_ReferenceCount++;

	m_TextureMap[name] = texture;

	return texture;
}

CTexture* CTextureHandler::CreateTextureFromSurface(SDL_Surface* surface, const std::string& name)
{
	if (!surface)
	{
		std::cout << "Error: failed to create texture from surface - surface is nullptr"  << std::endl;

		m_TextureMap.find("default")->second;
	}

	if (name.empty())
	{
		std::cout << "Error: failed to create texture from surface - name is empty"  << std::endl;

		m_TextureMap.find("default")->second;
	}

	CTexture* texture = new CTexture;

	if(!texture->CreateFromSurface(m_pRenderer, surface, name))
	{
		delete texture;
		texture = nullptr;

		return (!m_TextureMap.empty() ? m_TextureMap.find("default")->second : nullptr); 
	}

	texture->m_ReferenceCount++;

	m_TextureMap[name] = texture;

	return texture;
}

void CTextureHandler::DestroyTexture(const std::string& name)
{
	if(name.empty())
		return;

	TextureMap::iterator FindIt = m_TextureMap.find(name);

	if(FindIt == m_TextureMap.end())
	{
	#ifdef _DEBUG
		std::cout << "Error: failed to destroy texture '" << name.c_str() << "' - texture not found"  << std::endl;
	#endif

		return;
	}

	if(FindIt->second->m_ReferenceCount > 0)
		FindIt->second->m_ReferenceCount--;

	if(FindIt->second->m_ReferenceCount != 0)
		return;

	FindIt->second->Destroy();
	delete FindIt->second;
	FindIt->second = nullptr;

	m_TextureMap.erase(FindIt);
}