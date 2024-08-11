#include "Pch.h"
#include "Fireball.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

void CFireball::Kill(void)
{
	CGameObject::Kill();

	m_IsActive = false;
}

void CFireball::Render(void)
{
	m_pTexture->SetFlipMethod(m_FlipMethod);
	m_pTexture->SetAngle(m_Angle);

	CGameObject::Render();
}

void CFireball::Update(const float deltaTime)
{
	m_LifeTime += deltaTime;

	const float xVelocity = ((m_FlipMethod == SDL_RendererFlip::SDL_FLIP_NONE) ? m_Velocity : -m_Velocity);

	m_Angle += xVelocity * deltaTime;
	if (m_Angle > 360.0f)
		m_Angle = 0.0f;

	m_Rectangle.x += xVelocity * deltaTime;
	m_Rectangle.y  = m_StartPosition.y + (sinf(m_LifeTime * 8.0f) * 80.0f);

	m_Collider.x = m_Rectangle.x;
	m_Collider.y = m_Rectangle.y;

	const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

	if ((m_Collider.x < -m_Collider.w) || (m_Collider.x > windowSize.x) || (m_Collider.y > (windowSize.y - m_Collider.h)))
		Kill();
}

void CFireball::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
	for (CGameObject* obstacle : obstacles)
	{
		if (QuadVsQuad(m_Collider, obstacle->GetCollider()))
		{
			Kill();

			break;
		}
	}
}

void CFireball::Activate(const SDL_FPoint& spawnPosition, const SDL_RendererFlip flipMethod)
{
	m_Rectangle.x = spawnPosition.x;
	m_Rectangle.y = spawnPosition.y;

	m_Collider.x = m_Rectangle.x;
	m_Collider.y = m_Rectangle.y;

	m_StartPosition = spawnPosition;

	m_FlipMethod = flipMethod;

	m_Angle		= 0.0f;
	m_LifeTime	= 0.0f;

	m_IsDead	= false;
	m_IsActive	= true;
}