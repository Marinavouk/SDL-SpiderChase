#include "Pch.h"
#include "Chair.h"

#include "Application.h"

bool CChair::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint textureSize	= m_pTexture->GetSize();
	const SDL_FPoint newTextureSize = {textureSize.x * 0.7f, textureSize.y * 0.7f};

	m_pTexture->SetSize(newTextureSize);

	m_Rectangle = {position.x, position.y, newTextureSize.x, newTextureSize.y};

	m_Collider = {m_Rectangle.x + m_ColliderOffset.x, m_Rectangle.y + m_ColliderOffset.y, newTextureSize.x - 10.0f, 20.0f};

	return true;
}

void CChair::SetPosition(const SDL_FPoint& position)
{
	CGameObject::SetPosition(position);

	m_Collider.x = m_Rectangle.x + m_ColliderOffset.x;
	m_Collider.y = m_Rectangle.y + m_ColliderOffset.y;
}