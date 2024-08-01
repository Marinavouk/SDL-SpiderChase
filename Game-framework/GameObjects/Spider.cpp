
#include "Spider.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CSpider::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	if (!CGameObject::Create(textureFileName, position))
		return false;

	const SDL_FPoint frameSize = {64.0f, 64.0f};

	m_pAnimatorHanging	= new CAnimator;
	m_pAnimatorIdle		= new CAnimator;
	m_pAnimatorWalking	= new CAnimator;
	m_pAnimatorHanging->Set(	1,	0, 0, 4, frameSize, 0.0f, false,	CAnimator::EDirection::FORWARD);
	m_pAnimatorIdle->Set(		10, 0, 9, 0, frameSize, 7.0f, true,		CAnimator::EDirection::FORWARD);
	m_pAnimatorWalking->Set(	10, 0, 9, 3, frameSize, 7.0f, true,		CAnimator::EDirection::FORWARD);

	m_pCurrentAnimator = m_pAnimatorHanging;

	m_pTexture->SetSize({frameSize.x * m_Scale, frameSize.y * m_Scale});
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = {position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale};

	m_Collider = {m_Rectangle.x + m_ColliderOffset.x, m_Rectangle.y + m_ColliderOffset.y, 34.0f * m_Scale, 36.0f * m_Scale};

	m_ColliderOffset = {16.0f * m_Scale, 15.0f * m_Scale};

	m_Velocity.x = 130.0f;
	m_Velocity.y = 100.0f;

	m_StartPosition = {m_Rectangle.x, m_Rectangle.y};

	return true;
}

void CSpider::Destroy(void)
{
	delete m_pAnimatorWalking;
	delete m_pAnimatorIdle;
	delete m_pAnimatorHanging;
	m_pAnimatorWalking	= nullptr;
	m_pAnimatorIdle		= nullptr;
	m_pAnimatorHanging	= nullptr;
	m_pCurrentAnimator	= nullptr;

	CGameObject::Destroy();
}

void CSpider::Render(void)
{
	if ((m_State == EState::MOVING_DOWN_FROM_CEILING) || (m_State == EState::HANGING_IN_THREAD))
	{
		SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 200);
		SDL_RenderDrawLineF(renderer, m_StartPosition.x + (m_Rectangle.w * 0.5f), 0.0f, m_Collider.x + (m_Collider.w * 0.5f), m_Collider.y + (m_Collider.h * 0.5f));
	}

	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());
	m_pTexture->SetFlipMethod(m_FlipMethod);
	m_pTexture->SetAngle(-m_Angle);

	CGameObject::Render();
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

		ActivateHangingAnimation();

		m_Angle = 0.0f;

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

		m_Angle = sinf(m_LifeTime * 1.5f) * 15.0f;

		m_Rectangle.x = m_StartPosition.x + (m_Angle * 2.0f);

		SyncCollider();

		ActivateHangingAnimation();

		if (m_pTarget)
		{
			// Check if the player is under the spider
			// If the player is under this spider, switch to the FALLING_DOWN state
			if (fabs(m_pTarget->GetColliderPosition().x - m_Collider.x) <= 40.0f)
			{
				m_Angle = 0.0f;

				m_State = EState::FALLING_DOWN;
			}
		}
	}

	else if (m_State == EState::FALLING_DOWN)
	{
		m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);

		m_Rectangle.y += m_Velocity.y * deltaTime;

		SyncCollider();

		ActivateHangingAnimation();

		const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

		if (m_Collider.y > windowSize.y - m_Collider.h)
		{
			const float bottomOffset = m_Rectangle.h - (m_Collider.h + m_ColliderOffset.y);

			m_Rectangle.y = windowSize.y - (m_Rectangle.h - bottomOffset);

			SyncCollider();

			m_Velocity.y = 0.0f;

			m_Angle = 0.0f;

			m_State = EState::CHASING_PLAYER;
		}
	}

	else if (m_State == EState::CHASING_PLAYER)
	{
		m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);

		m_Rectangle.y += m_Velocity.y * deltaTime;

		SyncCollider();

		const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

		if (m_Collider.y > windowSize.y - m_Collider.h)
		{
			const float bottomOffset = m_Rectangle.h - (m_Collider.h + m_ColliderOffset.y);

			m_Rectangle.y = windowSize.y - (m_Rectangle.h - bottomOffset);

			SyncCollider();

			m_Velocity.y = 0.0f;

			m_Angle = 0.0f;
		}

		const SDL_FPoint playerPosition = m_pTarget->GetColliderCenterPosition();
		const SDL_FPoint centerPosition = GetColliderCenterPosition();
	
		if (m_pTarget)
		{	
			if (centerPosition.x > (playerPosition.x + 10.0f))
			{
				m_Rectangle.x -= m_Velocity.x * deltaTime;

				m_FlipMethod = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

				ActivateWalkingAnimation();
			}

			else if (centerPosition.x < (playerPosition.x - 10.0f))
			{
				m_Rectangle.x += m_Velocity.x * deltaTime;

				m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;

				ActivateWalkingAnimation();
			}

			else
				ActivateIdleAnimation();

			SyncCollider();
		}
	}

	if (m_pCurrentAnimator)
		m_pCurrentAnimator->Update(deltaTime);
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

		m_Angle = 0.0f;

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

void CSpider::ActivateHangingAnimation(void)
{
	if (m_pCurrentAnimator != m_pAnimatorHanging)
	{
		m_pCurrentAnimator = m_pAnimatorHanging;
		m_pCurrentAnimator->Reset();
	}
}

void CSpider::ActivateIdleAnimation(void)
{
	if (m_pCurrentAnimator != m_pAnimatorIdle)
	{
		m_pCurrentAnimator = m_pAnimatorIdle;
		m_pCurrentAnimator->Reset();
	}
}

void CSpider::ActivateWalkingAnimation(void)
{
	if (m_pCurrentAnimator != m_pAnimatorWalking)
	{
		m_pCurrentAnimator = m_pAnimatorWalking;
		m_pCurrentAnimator->Reset();
	}
}