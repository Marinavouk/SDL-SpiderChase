#include "Player.h"

#include "Application.h"

#include <SDL.h>

static bool QuadVsQuad(const SDL_FRect& rQuad1, const SDL_FRect& rQuad2, SDL_FRect* pIntersection = nullptr)
{
	SDL_FRect	Intersection	= {0.0f, 0.0f, 0.0f, 0.0f};
	const bool	Result			= (SDL_IntersectFRect(&rQuad1, &rQuad2, &Intersection) == SDL_TRUE);

	if(pIntersection)
	{
		pIntersection->x = Intersection.x;
		pIntersection->y = Intersection.y;
		pIntersection->w = Intersection.w;
		pIntersection->h = Intersection.h;
	}

	return Result;
}

bool CPlayer::Create(void)
{
	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture("Assets/Textures/character.png");
	if (!m_pTexture)
		return false;

	m_Rectangle = {300.0f, m_pApplication->GetWindow().GetSize().y - 128.0f, 64.0f * 2.0f, 128.0f * 2.0f};

	m_HorizontalCollider	= {m_Rectangle.x + m_HorizontalColliderOffset.x,	m_Rectangle.y + m_HorizontalColliderOffset.y,	18.0f * 2.0f, 40.0f * 2.0f};
	m_VerticalCollider		= {m_Rectangle.x + m_VerticalColliderOffset.x,		m_Rectangle.y + m_VerticalColliderOffset.y,		10.0f * 2.0f, 64.0f * 2.0f};

	return true;
}

void CPlayer::Destroy(void)
{
	CGameObject::Destroy();
}

void CPlayer::Update(const float deltaTime)
{
	const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

	m_Velocity.y += m_Gravity * deltaTime;

	m_Rectangle.x += m_Velocity.x * deltaTime;
	m_Rectangle.y += m_Velocity.y * deltaTime;

	SyncColliders();

	if(m_HorizontalCollider.x < 0.0f)
	{
		m_Rectangle.x = -m_HorizontalColliderOffset.x;

		SyncColliders();

		m_Velocity.x = 0.0f;
	}

	else if(m_HorizontalCollider.x > (windowSize.x - m_HorizontalCollider.w))
	{
		const float rightOffset = m_Rectangle.w - (m_HorizontalCollider.w + m_HorizontalColliderOffset.x);

		m_Rectangle.x = windowSize.x - (m_Rectangle.w - rightOffset);

		SyncColliders();

		m_Velocity.x = 0.0f;
	}

	if (m_Rectangle.y > windowSize.y - m_Rectangle.h)
	{
		m_Rectangle.y = windowSize.y - m_Rectangle.h;

		SyncColliders();

		m_Velocity.y = 0.0f;

		m_IsJumping = false;
	}
}

void CPlayer::Render(void)
{
	// Very temporary, will be removed when the character is animated
	// This is just to have anything rendered to the screen
	const SDL_Rect		clipRect	= {0, 0, 64, 128};
	const SDL_FPoint	size		= {64.0f * 2.0f, 128.0f * 2.0f};
	m_pApplication->GetTextureHandler().RenderTexture(m_pTexture, {m_Rectangle.x, m_Rectangle.y}, &clipRect, &size);
}

void CPlayer::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Rectangle);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_HorizontalCollider);

	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderDrawRectF(renderer, &m_VerticalCollider);
}

void CPlayer::HandleInput(const float deltaTime)
{
	CInputHandler& inputHandler = m_pApplication->GetInputHandler();

	// Pressed keys

	if (inputHandler.KeyPressed(SDL_SCANCODE_SPACE) && !m_IsJumping)
	{
		m_Velocity.y = -m_JumpStrength;

		m_IsJumping = true;
	}

	//////////////////////////////////////////////////////////////////////////

	// Held keys

	m_IsRunning = inputHandler.KeyHeld(SDL_SCANCODE_S);

	if (inputHandler.KeyHeld(SDL_SCANCODE_LEFT) && !inputHandler.KeyHeld(SDL_SCANCODE_RIGHT))
	{
		m_Velocity.x = std::max(m_Velocity.x - ((m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime), (m_IsRunning ? -m_MaxRunningVelocity : -m_MaxWalkingVelocity));

		m_LookDirection = 0;

		m_HorizontalDirection = EState::MOVING_LEFT;
	}

	else if (inputHandler.KeyHeld(SDL_SCANCODE_RIGHT) && !inputHandler.KeyHeld(SDL_SCANCODE_LEFT))
	{
		m_Velocity.x = std::min(m_Velocity.x + ((m_IsRunning ? m_AccelerationSpeedRunning : m_AccelerationSpeedWalking) * deltaTime), (m_IsRunning ? m_MaxRunningVelocity : m_MaxWalkingVelocity));

		m_LookDirection = 1;

		m_HorizontalDirection = EState::MOVING_RIGHT;
	}

	else
	{
		const float speed = (m_IsRunning ? m_DeaccelerationSpeedRunning : m_DeaccelerationSpeedWalking) * deltaTime;

		if (m_Velocity.x < 0.0f)
			m_Velocity.x = std::min(0.0f, m_Velocity.x + speed);

		else if (m_Velocity.x > 0.0f)
			m_Velocity.x = std::max(0.0f, m_Velocity.x - speed);
	}

	//////////////////////////////////////////////////////////////////////////

	// Released keys

	if (inputHandler.KeyReleased(SDL_SCANCODE_LEFT) && (m_HorizontalDirection == EState::MOVING_LEFT))
		m_HorizontalDirection = EState::IDLE;

	else if (inputHandler.KeyReleased(SDL_SCANCODE_RIGHT) && (m_HorizontalDirection == EState::MOVING_RIGHT))
		m_HorizontalDirection = EState::IDLE;
}

void CPlayer::HandleCollision(const std::vector<CGameObject*>& obstacles, const float deltaTime)
{
	const SDL_FPoint moveAmount = {m_Velocity.x * deltaTime, m_Velocity.y * deltaTime};

	for (CGameObject* obstacle : obstacles)
	{
		ResolveXCollision(obstacle->GetCollider(), moveAmount);
	}

	for (CGameObject* obstacle : obstacles)
	{
		ResolveYCollision(obstacle->GetCollider(), moveAmount);
	}
}

void CPlayer::SyncColliders()
{
	m_HorizontalCollider.x	= m_Rectangle.x + m_HorizontalColliderOffset.x;
	m_HorizontalCollider.y	= m_Rectangle.y + m_HorizontalColliderOffset.y;
	m_VerticalCollider.x	= m_Rectangle.x + m_VerticalColliderOffset.x;
	m_VerticalCollider.y	= m_Rectangle.y + m_VerticalColliderOffset.y;
}

void CPlayer::ResolveXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	// The player is moving to the left
	if (moveAmount.x < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += intersection.w;

			m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;

			m_Velocity.x = 0.0f;
		}
	}

	// The player is moving to the right
	else if (moveAmount.x > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x -= intersection.w;

			m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;

			m_Velocity.x = 0.0f;
		}
	}

	if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_HorizontalCollider, collider, &intersection))
		{
			m_Rectangle.x += ((m_HorizontalCollider.x < collider.x) ? -intersection.w : intersection.w);

			m_HorizontalCollider.x = m_Rectangle.x + m_HorizontalColliderOffset.x;

			m_Velocity.x = 0.0f;
		}
	}
}

void CPlayer::ResolveYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount)
{
	// The player is moving up
	if (moveAmount.y < 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y += intersection.h;

			m_VerticalCollider.y = m_Rectangle.y + m_VerticalColliderOffset.y;

			m_Velocity.y = 0.0f;
		}
	}

	// The player is moving down
	else if (moveAmount.y > 0.0f)
	{
		SDL_FRect intersection = {0.0f, 0.0f, 0.0f, 0.0f};

		if (QuadVsQuad(m_VerticalCollider, collider, &intersection))
		{
			m_Rectangle.y -= intersection.h;

			m_VerticalCollider.y = m_Rectangle.y + m_VerticalColliderOffset.y;

			m_Velocity.y = 0.0f;

			m_IsJumping = false;
		}
	}
}