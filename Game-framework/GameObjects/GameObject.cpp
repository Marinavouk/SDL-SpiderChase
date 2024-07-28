#include "GameObject.h"

#include "Application.h"

bool CGameObject::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture(textureFileName); 

	return true;
}

void CGameObject::Destroy(void)
{
	m_pApplication->GetTextureHandler().DestroyTexture(m_pTexture->GetName());
	m_pTexture = nullptr;
}