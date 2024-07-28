#include "GameObject.h"

#include "Application.h"

bool CGameObject::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture("Assets/Textures/" + textureFileName);
	if (!m_pTexture)
		return false;

	return true;
}

void CGameObject::Destroy(void)
{
	m_pApplication->GetTextureHandler().DestroyTexture(m_pTexture);
	m_pTexture = nullptr;
}