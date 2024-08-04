#include "Fireball.h"

bool CFireball::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint textureSize	= m_pTexture->GetSize();
	const SDL_FPoint newTextureSize = {textureSize.x * 1.5f, textureSize.y * 1.5f};

	m_pTexture->SetSize(newTextureSize);

	m_Rectangle = {position.x, position.y, newTextureSize.x, newTextureSize.y};

	m_Collider = m_Rectangle;

	return true;
}


void CFireball::Update(const float deltaTime)
{
	m_Rectangle.x += 32.0f * deltaTime;

	m_Collider.x = m_Rectangle.x;
	m_Collider.y = m_Rectangle.y;
}

void CFireball::Activate(const SDL_FPoint& spawnPosition)
{
	m_Rectangle.x = spawnPosition.x;
	m_Rectangle.y = spawnPosition.y;

	m_IsActive = true;
}