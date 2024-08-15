#include "Pch.h"
#include "TextureHandler.h"

#include "Utilities/StringUtilities.h"

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
		return RetrieveTexture("default");

	const std::string name = RemoveFileExtension(fileName);

	CTexture* texture = RetrieveTexture(name);

	if (texture)
		return texture;

	texture = new CTexture;

	if(!texture->Create(m_pRenderer, m_BasePath + "/" + fileName))
	{
		delete texture;
		texture = nullptr;

		return RetrieveTexture("default");
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

		return RetrieveTexture("default");
	}

	if (name.empty())
	{
		std::cout << "Error: failed to create texture from surface - name is empty"  << std::endl;

		return RetrieveTexture("default");
	}

	CTexture* texture = RetrieveTexture(name);

	if (texture)
		return texture;

	texture = new CTexture;

	if(!texture->CreateFromSurface(m_pRenderer, surface, name))
	{
		delete texture;
		texture = nullptr;

		return RetrieveTexture("default");
	}

	texture->m_ReferenceCount++;

	m_TextureMap[name] = texture;

	return texture;
}

CTexture* CTextureHandler::CreateEmptyTexture(const SDL_Point& size, const SDL_TextureAccess textureAccess, const std::string& name)
{
	if (name.empty())
	{
		std::cout << "Error: failed to create texture from surface - name is empty"  << std::endl;

		return RetrieveTexture("default");
	}

	CTexture* texture = RetrieveTexture(name);

	if (texture)
		return texture;

	texture = new CTexture;

	if(!texture->CreateEmpty(m_pRenderer, size, textureAccess, name))
	{
		delete texture;
		texture = nullptr;

		return RetrieveTexture("default");
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

CTexture* CTextureHandler::RetrieveTexture(const std::string& name)
{
	TextureMap::const_iterator FindIt = m_TextureMap.find(name);

	if(FindIt != m_TextureMap.end())
	{
		FindIt->second->m_ReferenceCount++;

		return FindIt->second;
	}

	return nullptr;
}