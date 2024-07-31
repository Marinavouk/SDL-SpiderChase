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

	m_Rectangle = {position.x, position.y, 64.0f * m_Scale, 64.0f * m_Scale};

	m_Collider = {m_Rectangle.x + m_ColliderOffset.x, m_Rectangle.y + m_ColliderOffset.y, 34.0f * m_Scale, 36.0f * m_Scale};

	m_ColliderOffset = {16.0f * m_Scale, 15.0f * m_Scale};

	m_Velocity.x = 130.0f;
	m_Velocity.y = 100.0f;

	m_StartPosition = {m_Rectangle.x, m_Rectangle.y};

	return true;
}

void CSpider::Render(void)
{
	if ((m_State == EState::MOVING_DOWN_FROM_CEILING) || (m_State == EState::HANGING_IN_THREAD))
	{
		SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 200);
		SDL_RenderDrawLineF(renderer, m_StartPosition.x + (m_Rectangle.w * 0.5f), 0.0f, m_Collider.x + (m_Collider.w * 0.5f), m_Collider.y + (m_Collider.h * 0.5f));
	}

	// Very temporary, will be removed when the spider is animated
	// This is just to have something rendered to the screen
	m_pTexture->Render({m_Rectangle.x, m_Rectangle.y});
}

void CSpider::RenderDebug(void)
{
	CGameObject::RenderDebug();

	SDL_Renderer*		renderer		= m_pApplication->GetWindow().GetRenderer();
	const SDL_FPoint	centerPosition	= {m_Collider.x + (m_Collider.w * 0.5f), m_Collider.y + (m_Collider.w * 0.5f)};
	const SDL_FPoint	targetPosition	= m_pTarget->GetColliderCenterPosition();

		 if (centerPosition.x > targetPosition.x) SDL_SetRenderDrawColor(renderer, 255, 255,	255,	255);
	else if (centerPosition.x < targetPosition.x) SDL_SetRenderDrawColor(renderer, 255,	0,		0,		255);

	SDL_RenderDrawLineF(renderer, centerPosition.x, centerPosition.y, targetPosition.x, targetPosition.y);
}

void CSpider::Update(const float deltaTime)
{
	if (m_State == EState::MOVING_DOWN_FROM_CEILING)
	{
		m_Rectangle.y += m_Velocity.y * deltaTime;

		SyncCollider();

		if (m_Rectangle.y > 150.0f)
		{
			m_LifeTime = 0.0f;

			m_StartPosition = {m_Rectangle.x, m_Rectangle.y};

			m_State = EState::HANGING_IN_THREAD;
		}
	}

	else if (m_State == EState::HANGING_IN_THREAD)
	{
		m_LifeTime += deltaTime;

		const float angle = sinf(m_LifeTime * 1.5f) * 15.0f;

		m_pTexture->SetAngle(-angle);

		m_Rectangle.x = m_StartPosition.x + (angle * 2.0f);

		SyncCollider();

		if (m_pTarget)
		{
			// Check if the player is under the spider
			// If the player is under this spider, switch to the FALLING_DOWN state
			if (fabs(m_pTarget->GetColliderPosition().x - m_Collider.x) <= 40.0f)
			{
				m_pTexture->SetAngle(0.0f);

				m_State = EState::FALLING_DOWN;
			}
		}
	}

	else if (m_State == EState::FALLING_DOWN)
	{
		m_Velocity.y += m_Gravity * deltaTime;

		m_Rectangle.y += m_Velocity.y * deltaTime;

		SyncCollider();

		const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

		if (m_Collider.y > windowSize.y - m_Collider.h)
		{
			const float bottomOffset = m_Rectangle.h - (m_Collider.h + m_ColliderOffset.y);

			m_Rectangle.y = windowSize.y - (m_Rectangle.h - bottomOffset);

			SyncCollider();

			m_Velocity.y = 0.0f;

			m_State = EState::CHASING_PLAYER;
		}
	}

	else if (m_State == EState::CHASING_PLAYER)
	{
		const SDL_FPoint playerPosition = m_pTarget->GetColliderCenterPosition();
		const SDL_FPoint centerPosition = GetColliderCenterPosition();
	
		if (m_pTarget)
		{	
			if (centerPosition.x > (playerPosition.x + 10.0f))
				m_Rectangle.x -= m_Velocity.x * deltaTime;
			else if (centerPosition.x < (playerPosition.x - 10.0f))
				m_Rectangle.x += m_Velocity.x * deltaTime;
			
			SyncCollider();
		}
		
	}
}

void CSpider::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
	for (CGameObject* obstacle : obstacles)
	{
		if (ResolveObstacleYCollision(obstacle->GetCollider()))
		{
			if (m_State == EState::FALLING_DOWN)
				m_State = EState::CHASING_PLAYER;

			break;
		}
	}
}

bool CSpider::ResolveObstacleYCollision(const SDL_FRect& collider)
{
	bool hasCollided = false;

	SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

	if (QuadVsQuad(m_Collider, collider, &intersection))
	{
		m_Rectangle.y -= intersection.h;

		m_pTexture->SetAngle(0.0f);

		m_Velocity.y = 0.0f;

		SyncCollider();

		hasCollided = true;
	}

	return hasCollided;
}

void CSpider::SyncCollider(void)
{
	m_Collider.x = m_Rectangle.x + m_ColliderOffset.x;
	m_Collider.y = m_Rectangle.y + m_ColliderOffset.y;
}