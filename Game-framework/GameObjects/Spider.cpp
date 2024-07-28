#include "Spider.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CSpider::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	if (!CGameObject::Create(textureFileName, position))
		return false;

	if (m_pTexture->GetName() != "default")
	{
		m_pTexture->SetSize({64.0f * m_Scale, 64.0f * m_Scale});
		m_pTexture->SetTextureCoords(0, 64, 256, 320);
	}

	m_Velocity.x = 120.0f;

	m_Rectangle = {position.x, position.y, 64.0f * m_Scale, 64.0f * m_Scale};

	m_Collider = {m_Rectangle.x + m_ColliderOffset.x, m_Rectangle.y + m_ColliderOffset.y, 34.0f * m_Scale, 36.0f * m_Scale};

	return true;
}

void CSpider::Destroy(void)
{
	m_pTarget = nullptr;

	CGameObject::Destroy();
}

void CSpider::Update(const float deltaTime)
{
	m_Velocity.y += m_Gravity * deltaTime;

	m_Rectangle.x += m_Velocity.x * deltaTime;
	m_Rectangle.y += m_Velocity.y * deltaTime;

	SyncCollider();

	const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

	if (m_Collider.x < 0.0f)
	{
		m_Rectangle.x = -m_ColliderOffset.x;

		m_Velocity.x = -m_Velocity.x;
	}

	else if(m_Collider.x > (windowSize.x - m_Collider.w))
	{
		const float rightOffset = m_Rectangle.w - (m_Collider.w + m_ColliderOffset.x);

		m_Rectangle.x = windowSize.x - (m_Rectangle.w - rightOffset);

		m_Velocity.x = -m_Velocity.x;
	}

	if (m_Collider.y > windowSize.y - m_Collider.h)
	{
		const float bottomOffset = m_Rectangle.h - (m_Collider.h + m_ColliderOffset.y);

		m_Rectangle.y = windowSize.y - (m_Rectangle.h - bottomOffset);

		m_Velocity.y = 0.0f;
	}

	SyncCollider();
}

void CSpider::Render(void)
{
	// Very temporary, will be removed when the spider is animated
	// This is just to have anything rendered to the screen
	m_pTexture->Render({m_Rectangle.x, m_Rectangle.y});
}

void CSpider::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Rectangle);

	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider); 
}

void CSpider::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
	const SDL_FPoint moveAmount = {m_Velocity.x * deltaTime, m_Velocity.y * deltaTime};

	for (CGameObject* obstacle : obstacles)
	{
		if (ResolveObstacleYCollision(obstacle->GetCollider(), moveAmount))
			break;
	}
}

void CSpider::SetDirection(const int direction)
{
	m_pTexture->SetFlipMethod(((direction == 0) ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE));

	if ((direction == 0) && (m_Velocity.x > 0.0f))
		m_Velocity.x = -m_Velocity.x;
}

void CSpider::SyncCollider(void)
{
	m_Collider.x = m_Rectangle.x + m_ColliderOffset.x;
	m_Collider.y = m_Rectangle.y + m_ColliderOffset.y;
}

bool CSpider::ResolveObstacleYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

	if (QuadVsQuad(m_Collider, collider, &intersection))
	{
		m_Rectangle.y -= intersection.h;

		m_Velocity.y = 0.0f;

		hasCollided = true;
	}

	if (hasCollided)
		SyncCollider();

	return hasCollided;
}