#include "Fireball.h"

void CFireball::Update(const float deltaTime)
{
	m_Rectangle.x += 300.0f * deltaTime;

	m_Collider.x = m_Rectangle.x;
	m_Collider.y = m_Rectangle.y;
}

void CFireball::Activate(const SDL_FPoint& spawnPosition)
{
	m_Rectangle.x = spawnPosition.x;
	m_Rectangle.y = spawnPosition.y;

	m_Collider.x = m_Rectangle.x;
	m_Collider.y = m_Rectangle.y;

	m_IsActive = true;
}