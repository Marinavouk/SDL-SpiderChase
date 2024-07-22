#pragma once

#include "GameObject.h"

#include <SDL.h>
#include <vector>

class CPlayer final : public CGameObject
{
public:

	 CPlayer(void)													{}
	 CPlayer(CApplication* application) : CGameObject(application)	{}
	~CPlayer(void)													{}

	virtual bool Create(void) override;
	virtual void Destroy(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;
	virtual void HandleInput(const float deltaTime) override;
	virtual void HandleCollision(const std::vector<CGameObject*>& obstacles, const float deltaTime) override;

private:

	void SyncColliders();
	void ResolveXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	void ResolveYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);

private:

	enum EState
	{
		IDLE = -1,
		MOVING_LEFT,
		MOVING_RIGHT,
	};

private:

	float		m_AccelerationSpeedWalking		= 800.0f;
	float		m_DeaccelerationSpeedWalking	= 500.0f;
	float		m_AccelerationSpeedRunning		= 1000.0f;
	float		m_DeaccelerationSpeedRunning	= 700.0f;
	float		m_MaxWalkingVelocity			= 200.0f;
	float		m_MaxRunningVelocity			= 400.0f;
	float		m_JumpStrength					= 660.0f;
	float		m_Gravity						= 1500.0f;

	// 0 = facing left
	// 1 = facing right
	int			m_LookDirection					= 1;

	int			m_HorizontalDirection			= EState::IDLE;
	int			m_VerticalDirection				= EState::IDLE;

	SDL_FPoint	m_Velocity						= {0.0f, 0.0f};

	SDL_FRect	m_HorizontalCollider			= {0.0f, 0.0f, 0.0f, 0.0f};
	SDL_FRect	m_VerticalCollider				= {0.0f, 0.0f, 0.0f, 0.0f};

	bool		m_IsRunning						= false;
	bool		m_IsJumping						= false;


	//////////////////////////////////////////////////////////////////////////
	// Temporary data
	SDL_FPoint m_HorizontalColliderOffset	= {24.0f * 2.0f, 78.0f * 2.0f};
	SDL_FPoint m_VerticalColliderOffset		= {28.0f * 2.0f, 64.0f * 2.0f};
	//////////////////////////////////////////////////////////////////////////

};