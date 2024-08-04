#include "CFireball.h"

void CFireball::Update(const float deltaTime)
{
	m_Rectangle.x += 4.0f * deltaTime;

	m_Collider.x = m_Rectangle.x;
	m_Collider.y = m_Rectangle.y;
}