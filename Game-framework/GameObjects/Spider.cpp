#include "Pch.h"
#include "Spider.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

bool CSpider::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = {64.0f, 64.0f};

	m_pAnimatorHanging	= new CAnimator;
	m_pAnimatorIdle		= new CAnimator;
	m_pAnimatorWalking	= new CAnimator;
	m_pAnimatorDying	= new CAnimator;
	m_pAnimatorHanging->Set(	m_pTexture, 1,	0, 0, 4, frameSize, 0.0f, false,	CAnimator::EDirection::FORWARD);
	m_pAnimatorIdle->Set(		m_pTexture, 10, 0, 9, 0, frameSize, 7.0f, true,		CAnimator::EDirection::FORWARD);
	m_pAnimatorWalking->Set(	m_pTexture, 10, 0, 9, 3, frameSize, 7.0f, true,		CAnimator::EDirection::FORWARD);
	m_pAnimatorDying->Set(		m_pTexture, 4,  0, 3, 4, frameSize, 7.0f, false,	CAnimator::EDirection::FORWARD);

	m_pAnimatorDying->SetAnimationEndCallback(std::bind(&CSpider::OnDyingAnimationEnd, this));

	ActivateAnimator(m_pAnimatorHanging);

	m_pTexture->SetSize({frameSize.x * m_Scale, frameSize.y * m_Scale});
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = {position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale};

	m_Collider = {m_Rectangle.x + m_ColliderOffset.x, m_Rectangle.y + m_ColliderOffset.y, 34.0f * m_Scale, 36.0f * m_Scale};

	m_StartPosition = {m_Rectangle.x, m_Rectangle.y};

	return true;
}

void CSpider::Destroy(void)
{
	m_pDyingCallback = nullptr;

	delete m_pAnimatorDying;
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

void CSpider::Kill(void)
{
	CGameObject::Kill();

	ActivateAnimator(m_pAnimatorDying);

	m_Velocity = {0.0f, 0.0f};

	m_State = EState::DEAD;
}

void CSpider::Update(const float deltaTime)
{
	if (m_State == EState::MOVING_DOWN_FROM_CEILING)
	{
		m_Rectangle.y += m_ThreadMoveVelocity * deltaTime;

		SyncCollider();

		if (m_Rectangle.y > m_ThreadLength)
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

		if (m_pTarget)
		{
			// Check if the player is under the spider, within a certain threshold
			// If that's the case, switch to the FALLING_DOWN state
			if (fabs(m_pTarget->GetColliderPosition().x - m_Collider.x) <= 40.0f)
			{
				m_Angle = 0.0f;

				m_Velocity.y = 0.0f;

				m_State = EState::FALLING_DOWN;
			}
		}
	}

	else
	{
		m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);

		m_Rectangle.y += m_Velocity.y * deltaTime;

		SyncCollider();

		const SDL_FPoint windowSize = m_pApplication->GetWindowSize();

		if (m_Collider.y > windowSize.y - m_Collider.h)
		{
			const float bottomOffset = m_Rectangle.h - (m_Collider.h + m_ColliderOffset.y);

			m_Rectangle.y = windowSize.y - (m_Rectangle.h - bottomOffset);

			SyncCollider();

			m_Velocity.y = 0.0f;

			if (m_State != EState::DEAD)
				m_State = EState::CHASING_PLAYER;
		}
	}

	if (m_State == EState::CHASING_PLAYER)
	{
		if (m_pTarget && !m_IsDead)
		{	
			const SDL_FPoint playerPosition = m_pTarget->GetColliderCenterPosition();
			const SDL_FPoint centerPosition = GetColliderCenterPosition();

			if (centerPosition.x > (playerPosition.x + 10.0f))
			{
				m_Rectangle.x -= m_Velocity.x * deltaTime;

				m_FlipMethod = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;

				ActivateAnimator(m_pAnimatorWalking);
			}

			else if (centerPosition.x < (playerPosition.x - 10.0f))
			{
				m_Rectangle.x += m_Velocity.x * deltaTime;

				m_FlipMethod = SDL_RendererFlip::SDL_FLIP_NONE;

				ActivateAnimator(m_pAnimatorWalking);
			}

			else
				ActivateAnimator(m_pAnimatorIdle);

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

void CSpider::Activate(const SDL_FPoint& spawnPosition, const float threadLength, const uint32_t index)
{
	ActivateAnimator(m_pAnimatorHanging);

	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle.x = spawnPosition.x;
	m_Rectangle.y = spawnPosition.y;

	SyncCollider();

	m_StartPosition = spawnPosition;

	m_Angle			= 0.0f;
	m_LifeTime		= 0.0f;
	m_ThreadLength	= threadLength;

	m_Index = index;

	m_IsActive	= true;
	m_IsDead	= false;

	m_Velocity.x = m_ChaseVelocity;

	m_State = EState::MOVING_DOWN_FROM_CEILING;
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

void CSpider::ActivateAnimator(CAnimator* animator)
{
	if (m_pCurrentAnimator != animator)
	{
		m_pCurrentAnimator = animator;
		m_pCurrentAnimator->Reset();
	}
}

void CSpider::OnDyingAnimationEnd(void)
{
	if (!m_IsActive)
		return;

	if (m_pDyingCallback)
		m_pDyingCallback(m_Index);

	m_IsActive = false;
}