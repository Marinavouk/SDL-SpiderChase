#include "CFireball.h"

void CFireball::Update(const float deltaTime)
{
	m_Rectangle.x += 4.0f * deltaTime;
}
