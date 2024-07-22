#include "GameObject.h"

#include "Application.h"

void CGameObject::Destroy(void)
{
	m_pApplication->GetTextureHandler().DestroyTexture(m_pTexture);
	m_pTexture = nullptr;
}