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

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	Render(void) override;
	virtual void	RenderDebug(void) override;
	virtual void	HandleInput(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;
	virtual void	HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime) override;

private:

	void			SyncColliders(void);
	void			ActivateDamageCooldown(void);
	bool			ResolveObstacleXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveObstacleYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveEnemyXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveEnemyYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);

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

	// How much the player should be moved out of the enemy it's colliding with
	float		m_HorizontalHitStrength			= 50.0f;

	// How much the player should jump when colliding with an enemy
	float		m_VerticalHitStrength			= 300.0f;

	float		m_Gravity						= 1500.0f;

	// How long the damage cooldown should last (2.0f = 2 seconds)
	float		m_DamageCooldownTimerDefault	= 2.0f;
	float		m_DamageCooldownTimer			= m_DamageCooldownTimerDefault;

	// How fast the player should blink while being in the damage-cooldown state
	// The lower the value, the faster the player will blink
	float		m_BlinkingIntervalDefault		= 0.005f;
	float		m_BlinkingInterval				= m_BlinkingIntervalDefault;

	int			m_HorizontalDirection			= EState::IDLE;
	int			m_VerticalDirection				= EState::IDLE;

	SDL_FPoint	m_Velocity						= {0.0f, 0.0f};

	SDL_FRect	m_HorizontalCollider			= {0.0f, 0.0f, 0.0f, 0.0f};
	SDL_FRect	m_VerticalCollider				= {0.0f, 0.0f, 0.0f, 0.0f};

	bool		m_IsRunning						= false;
	bool		m_IsJumping						= false;
	bool		m_DamageCooldown				= false;
	bool		m_Show							= true;


	//////////////////////////////////////////////////////////////////////////
	// Temporary data
	float		m_Scale						= 2.0f;
	SDL_FPoint	m_HorizontalColliderOffset	= {24.0f * m_Scale, 78.0f * m_Scale};
	SDL_FPoint	m_VerticalColliderOffset	= {28.0f * m_Scale, 64.0f * m_Scale};
	//////////////////////////////////////////////////////////////////////////

};