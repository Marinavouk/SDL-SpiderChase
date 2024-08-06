#include "Player.h"

#include "Application.h"
#include "Utilities/CollisionUtilities.h"

#include <SDL.h>
#include <iostream>

bool CPlayer::Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth)
{
	if (!CGameObject::Create(textureFileName, position, maxHealth))
		return false;

	const SDL_FPoint frameSize = {64.0f, 128.0f};

	m_pAnimatorIdle			= new CAnimator;
	m_pAnimatorWalking		= new CAnimator;
	m_pAnimatorRunning		= new CAnimator;
	m_pAnimatorJumping		= new CAnimator;
	m_pAnimatorAttacking	= new CAnimator;
	m_pAnimatorDead			= new CAnimator;
	m_pAnimatorIdle->Set(		7, 0, 6, 0, frameSize,  7.0f, true,		CAnimator::EDirection::FORWARD);
	m_pAnimatorWalking->Set(	6, 0, 5, 1, frameSize,  8.0f, true,		CAnimator::EDirection::FORWARD);
	m_pAnimatorRunning->Set(	8, 0, 7, 2, frameSize, 14.0f, true,		CAnimator::EDirection::FORWARD);
	m_pAnimatorJumping->Set(	9, 0, 8, 7, frameSize,  8.0f, false,	CAnimator::EDirection::FORWARD);
	m_pAnimatorAttacking->Set(	8, 0, 7, 5, frameSize, 14.0f, false,	CAnimator::EDirection::FORWARD);
	m_pAnimatorDead->Set(		6, 0, 5, 9, frameSize,	9.0f, false,	CAnimator::EDirection::FORWARD);

	m_pAnimatorAttacking->SetAnimationEndCallback(std::bind(&CPlayer::OnAttackAnimationEnd, this));

	m_pCurrentAnimator = m_pAnimatorIdle;

	m_pTexture->SetSize({frameSize.x * m_Scale, frameSize.y * m_Scale});
	m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());

	m_Rectangle = {position.x, position.y, frameSize.x * m_Scale, frameSize.y * m_Scale};

	m_HorizontalColliderOffset	= {24.0f * m_Scale, 78.0f * m_Scale};
	m_VerticalColliderOffset	= {28.0f * m_Scale, 64.0f * m_Scale};

	m_HorizontalCollider	= {m_Rectangle.x + m_HorizontalColliderOffset.x,	m_Rectangle.y + m_HorizontalColliderOffset.y,	18.0f * m_Scale, 40.0f * m_Scale};
	m_VerticalCollider		= {m_Rectangle.x + m_VerticalColliderOffset.x,		m_Rectangle.y + m_VerticalColliderOffset.y,		10.0f * m_Scale, 64.0f * m_Scale};

	m_Collider = {m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h};

	m_HitStrength = {50.0f, 300.0f};

	return true;
}


void CPlayer::Destroy(void)
{
	delete m_pAnimatorDead;
	delete m_pAnimatorAttacking;
	delete m_pAnimatorJumping;
	delete m_pAnimatorRunning;
	delete m_pAnimatorWalking;
	delete m_pAnimatorIdle;
	m_pAnimatorDead			= nullptr;
	m_pAnimatorAttacking	= nullptr;
	m_pAnimatorJumping		= nullptr;
	m_pAnimatorRunning		= nullptr;
	m_pAnimatorWalking		= nullptr;
	m_pAnimatorIdle			= nullptr;
	m_pCurrentAnimator		= nullptr;

	CGameObject::Destroy();
}

void CPlayer::Update(const float deltaTime)
{
	const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

	m_Velocity.y = std::min(m_Velocity.y + m_Gravity * deltaTime, m_MaxFallVelocity);

	m_Rectangle.x += m_Velocity.x * deltaTime;
	m_Rectangle.y += m_Velocity.y * deltaTime;

	SyncColliders();

	if (m_HorizontalCollider.x < 0.0f)
	{
		m_Rectangle.x = -m_HorizontalColliderOffset.x;

		m_Velocity.x = 0.0f;
	}

	else if (m_HorizontalCollider.x > (windowSize.x - m_HorizontalCollider.w))
	{
		const float rightOffset = m_Rectangle.w - (m_HorizontalCollider.w + m_HorizontalColliderOffset.x);

		m_Rectangle.x = windowSize.x - (m_Rectangle.w - rightOffset);

		m_Velocity.x = 0.0f;
	}

	if (m_Rectangle.y > windowSize.y - m_Rectangle.h)
	{
		m_Rectangle.y = windowSize.y - m_Rectangle.h;

		m_Velocity.y = 0.0f;

		if (m_IsJumping && !m_IsAttacking)
		{
			if ((m_pCurrentAnimator != m_pAnimatorIdle) && (m_HorizontalDirection == EState::IDLE) && (m_VerticalDirection == EState::IDLE))
				ActivateIdleAnimation();

			else
			{
				if (m_HorizontalDirection != EState::IDLE)
				{
					if (m_IsRunning)
						ActivateRunningAnimation();

					else
						ActivateWalkingAnimation();
				}
			}
		}

		m_IsJumping = false;
	}

	SyncColliders();

	if (m_pCurrentAnimator)
	{
		m_pCurrentAnimator->Update(deltaTime);

		m_pTexture->SetTextureCoords(m_pCurrentAnimator->GetClipRectangle());
	}

	if (m_DamageCooldown)
	{
		m_DamageCooldownTimer -= deltaTime;

		if (m_DamageCooldownTimer <= 0.0f)
		{
			m_DamageCooldown = false;

			return;
		}

		m_BlinkingInterval -= deltaTime;

		if (m_BlinkingInterval <= 0.0f)
		{
			m_BlinkingInterval = m_BlinkingIntervalDefault;

			m_Show = !m_Show;
		}
	}
}

void CPlayer::Render(void)
{
	if (m_DamageCooldown && !m_Show)
		return;

	CGameObject::Render();
}

void CPlayer::RenderDebug(void)
{
	CGameObject::RenderDebug();

	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 255, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_HorizontalCollider);

	SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
	SDL_RenderDrawRectF(renderer, &m_VerticalCollider);
}

void CPlayer::HandleInput(const float deltaTime)
{
	CInputHandler& inputHandler = m_pApplication->GetInputHandler();

	// Pressed keys

	if (inputHandler.KeyPressed(SDL_SCANCODE_A) && !m_IsAttacking)
	{
		m_pCurrentAnimator = m_pAnimatorAttacking;
		m_pCurrentAnimator->Reset();

		m_IsAttacking = true;
	}

	if (inputHandler.KeyPressed(SDL_SCANCODE_SPACE) && !m_IsJumping)
	{
		m_Velocity.y = -m_JumpStrength;

		if (!m_IsAttacking)
		{
			m_pCurrentAnimator = m_pAnimatorJumping;
			m_pCurrentAnimator->Reset();
		}

		m_IsJumping = true;
	}

	//////////////////////////////////////////////////////////////////////////

	// Held keys

	m_IsRunning = inputHandler.KeyHeld(SDL_SCANCODE_S);

	if (inputHandler.KeyHeld(SDL_SCANCODE_LEFT) && !inputHandler.KeyHeld(SDL_SCANCODE_RIGHT))
	{
		m_Velocity.x = std::max(m_Velocity.x - ((m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime), (m_IsRunning ? -m_MaxRunningVelocity : -m_MaxWalkingVelocity));

		m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

		m_HorizontalDirection = EState::MOVING_LEFT;

		if (!m_IsJumping && !m_IsAttacking)
		{
			if (m_IsRunning)
				ActivateRunningAnimation();

			else
				ActivateWalkingAnimation();
		}
	}

	else if (inputHandler.KeyHeld(SDL_SCANCODE_RIGHT) && !inputHandler.KeyHeld(SDL_SCANCODE_LEFT))
	{
		m_Velocity.x = std::min(m_Velocity.x + ((m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime), (m_IsRunning ? m_MaxRunningVelocity : m_MaxWalkingVelocity));

		m_pTexture->SetFlipMethod(SDL_RendererFlip::SDL_FLIP_NONE);

		m_HorizontalDirection = EState::MOVING_RIGHT;

		if (!m_IsJumping && !m_IsAttacking)
		{
			if (m_IsRunning)
				ActivateRunningAnimation();

			else
				ActivateWalkingAnimation();
		}
	}

	else
	{
		const float speed = (m_IsRunning ? m_DeaccelerationSpeedRunning : m_DeaccelerationSpeedWalking) * deltaTime;

			 if (m_Velocity.x < 0.0f) m_Velocity.x = std::min(0.0f, m_Velocity.x + speed);
		else if (m_Velocity.x > 0.0f) m_Velocity.x = std::max(0.0f, m_Velocity.x - speed);
	}

	//////////////////////////////////////////////////////////////////////////

	// Released keys

	if (inputHandler.KeyReleased(SDL_SCANCODE_LEFT) && (m_HorizontalDirection == EState::MOVING_LEFT))
	{
		m_HorizontalDirection = EState::IDLE;

		if((m_VerticalDirection == EState::IDLE) && !m_IsJumping && !m_IsAttacking)
			ActivateIdleAnimation();
	}

	else if (inputHandler.KeyReleased(SDL_SCANCODE_RIGHT) && (m_HorizontalDirection == EState::MOVING_RIGHT))
	{
		m_HorizontalDirection = EState::IDLE;

		if((m_VerticalDirection == EState::IDLE) && !m_IsJumping && !m_IsAttacking)
			ActivateIdleAnimation();
	}

	if ((m_HorizontalDirection == EState::IDLE) && (m_VerticalDirection == EState::IDLE) && !m_IsJumping && !m_IsAttacking)
		ActivateIdleAnimation();
}

void CPlayer::HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)
{
	const SDL_FPoint moveAmount = {m_Velocity.x * deltaTime, m_Velocity.y * deltaTime};

	for (CGameObject* obstacle : obstacles)
	{
		if (ResolveObstacleXCollision(obstacle->GetCollider(), moveAmount))
			break;

		if (ResolveObstacleYCollision(obstacle->GetCollider(), moveAmount))
			break;
	}
}

void CPlayer::HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime)
{
	if (m_DamageCooldown)
		return;

	const SDL_FPoint	moveAmount	= {m_Velocity.x * deltaTime, m_Velocity.y * deltaTime};
	bool				hasCollided = false;

	for (CGameObject* enemy : enemies)
	{
		if (ResolveEnemyXCollision(enemy->GetCollider(), moveAmount))
		{
			hasCollided = true;

			break;
		}

		if (ResolveEnemyYCollision(enemy->GetCollider(), moveAmount))
		{
			hasCollided = true;

			break;
		}
	}

	if (hasCollided)
	{
		if (m_CurrentHealth > 0)
		{
			m_CurrentHealth--;

			if (m_CurrentHealth == 0)
			{
			#if defined(_DEBUG) 
				std::cout << "The player's health is now zero and the player should die" << std::endl;
			#endif

				// TODO: activate the player's death animation here
				ActivateDeathAnimation();
			}

			else
				ActivateDamageCooldown();
		}
	}
}

bool CPlayer::ResolveObstacleXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving to the left
	if (moveAmount.x < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += intersection.w;

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	// The player is moving to the right
	else if (moveAmount.x > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x -= intersection.w;

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += ((m_HorizontalCollider.x < collider.x) ? -intersection.w : intersection.w);

			SyncColliders();

			m_Velocity.x = 0.0f;

			hasCollided = true;
		}
	}

	return hasCollided;
}

bool CPlayer::ResolveObstacleYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving up (jumping)
	if (moveAmount.y < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y += intersection.h;

			m_Velocity.y = 0.0f;

			hasCollided = true;
		}
	}

	// The player is moving down
	else if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y -= intersection.h;

			m_Velocity.y = 0.0f;

			m_IsJumping = false;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

bool CPlayer::ResolveEnemyXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving to the left
	if (moveAmount.x < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += intersection.w;

			m_Velocity.x  = -m_Velocity.x;
			m_Velocity.x +=  m_HitStrength.x;
			m_Velocity.y -=  m_HitStrength.y;

			hasCollided = true;
		}
	}

	// The player is moving to the right
	else if (moveAmount.x > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x -= intersection.w;

			m_Velocity.x  = -m_Velocity.x;
			m_Velocity.x -=	 m_HitStrength.x;
			m_Velocity.y -=	 m_HitStrength.y;

			hasCollided = true;
		}
	}

	else
	{
		if (QuadVsQuad(m_HorizontalCollider, collider))
		{
			const float hitStrength = (m_HitStrength.x * 4.0f);

			m_Velocity.x += ((m_HorizontalCollider.x < collider.x) ? -hitStrength : hitStrength);
			m_Velocity.y -= m_HitStrength.y;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += ((m_HorizontalCollider.x < collider.x) ? -intersection.w	: intersection.w);

			SyncColliders();

			const float hitStrength = (m_HitStrength.x * 4.0f);

			m_Velocity.x  = -m_Velocity.x;
			m_Velocity.x +=  ((m_Velocity.x < 0.0f) ? -hitStrength : hitStrength);
			m_Velocity.y -=  m_HitStrength.y;

			hasCollided = true;
		}
	}

	return hasCollided;
}

bool CPlayer::ResolveEnemyYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	bool hasCollided = false;

	// The player is moving up (jumping)
	if (moveAmount.y < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y += intersection.h;

			const float hitStrength = (m_HitStrength.x * 2.0f);

			m_Velocity.x  = -m_Velocity.x;
			m_Velocity.x +=  ((m_Velocity.x < 0.0f) ? -hitStrength : hitStrength);

			hasCollided = true;
		}
	}

	// The player is moving down
	else if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y -= intersection.h;

			const float hitStrength = (m_HitStrength.x * 2.0f);

			m_Velocity.x  = -m_Velocity.x;
			m_Velocity.x += ((m_Velocity.x < 0.0f) ? -hitStrength : hitStrength);
			m_Velocity.y -= m_HitStrength.y;

			hasCollided = true;
		}
	}

	if (hasCollided)
		SyncColliders();

	return hasCollided;
}

void CPlayer::SyncColliders(void)
{
	m_HorizontalCollider.x	= m_Rectangle.x + m_HorizontalColliderOffset.x;
	m_HorizontalCollider.y	= m_Rectangle.y + m_HorizontalColliderOffset.y;
	m_VerticalCollider.x	= m_Rectangle.x + m_VerticalColliderOffset.x;
	m_VerticalCollider.y	= m_Rectangle.y + m_VerticalColliderOffset.y;

	m_Collider = {m_VerticalCollider.x, m_VerticalCollider.y, m_VerticalCollider.w, m_VerticalCollider.h};
}

void CPlayer::ActivateDamageCooldown(void)
{
	m_DamageCooldownTimer	= m_DamageCooldownTimerDefault;
	m_BlinkingInterval		= m_BlinkingIntervalDefault;
	m_DamageCooldown		= true;
	m_Show					= true;
}

void CPlayer::ActivateIdleAnimation(void)
{
	if (m_pCurrentAnimator != m_pAnimatorIdle)
	{
		m_pCurrentAnimator = m_pAnimatorIdle;
		m_pCurrentAnimator->Reset();
	}
}

void CPlayer::ActivateWalkingAnimation(void)
{
	if (m_pCurrentAnimator != m_pAnimatorWalking)
	{
		m_pCurrentAnimator = m_pAnimatorWalking;
		m_pCurrentAnimator->Reset();
	}
}

void CPlayer::ActivateRunningAnimation(void)
{
	if (m_pCurrentAnimator != m_pAnimatorRunning)
	{
		m_pCurrentAnimator = m_pAnimatorRunning;
		m_pCurrentAnimator->Reset();
	}
}

void CPlayer::OnAttackAnimationEnd(void)
{
	if (m_pAttackCallback)
		m_pAttackCallback();

	if (m_IsJumping)
		m_pCurrentAnimator = m_pAnimatorJumping;

	else
	{
		if (m_HorizontalDirection != EState::IDLE)
		{
			if (m_IsRunning)
				m_pCurrentAnimator = m_pAnimatorRunning;

			else
				m_pCurrentAnimator = m_pAnimatorRunning;
		}

		else
			m_pCurrentAnimator = m_pAnimatorIdle;
	}

	m_pCurrentAnimator->Reset();

	m_IsAttacking = false;
}

void CPlayer::ActivateDeathAnimation(void)
{
	if (m_pCurrentAnimator != m_pAnimatorDead) 
	{
		m_pCurrentAnimator = m_pAnimatorDead;
		m_pCurrentAnimator->Reset();
	}
}
