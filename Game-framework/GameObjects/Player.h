#pragma once

#include "GameObject.h"
#include "Utilities\Animator.h"

#include <SDL.h>
#include <vector>

class CPlayer final : public CGameObject
{
public:

	 CPlayer(void)													{}
	 CPlayer(CApplication* application) : CGameObject(application)	{}
	~CPlayer(void)													{}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void	Destroy(void) override;
	virtual void	Render(void) override;
	virtual void	RenderDebug(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleInput(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;
	virtual void	HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime) override;

private:

	bool			ResolveObstacleXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveObstacleYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveEnemyXCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	bool			ResolveEnemyYCollision(const SDL_FRect& collider, const SDL_FPoint& moveAmount);
	void			SyncColliders(void);
	void			ActivateDamageCooldown(void);
	void			ActivateIdleAnimation(void);
	void			ActivateWalkingAnimation(void);
	void			ActivateRunningAnimation(void);
	void			ActivateJumpingAnimation(void);
	void			OnAttackAnimationEnd(void);


private:

	enum EState
	{
		IDLE = -1,
		MOVING_LEFT,
		MOVING_RIGHT,
	};

private:

	CAnimator*	m_pAnimatorIdle					= nullptr;
	CAnimator*	m_pAnimatorWalking				= nullptr;
	CAnimator*	m_pAnimatorRunning				= nullptr;
	CAnimator*	m_pAnimatorJumping				= nullptr;
	CAnimator*	m_pAnimatorAttacking			= nullptr;
	CAnimator*	m_pCurrentAnimator				= nullptr;

	float		m_AccelerationSpeedWalking		= 800.0f;
	float		m_DeaccelerationSpeedWalking	= 500.0f;
	float		m_AccelerationSpeedRunning		= 1000.0f;
	float		m_DeaccelerationSpeedRunning	= 700.0f;
	float		m_MaxWalkingVelocity			= 200.0f;
	float		m_MaxRunningVelocity			= 400.0f;
	float		m_MaxFallVelocity				= 600.0f;
	float		m_JumpStrength					= 660.0f;

	// How much (in pixels) the player should be moved out of the spider it's colliding with
	float		m_HorizontalHitStrength			= 50.0f;

	// How high (in pixels) the player should jump when colliding with a spider
	float		m_VerticalHitStrength			= 300.0f;

	float		m_Gravity						= 1500.0f;
	float		m_Scale							= 2.0f;

	// How long time (in seconds) the damage-cooldown should last
	// During the damage-cooldown, the player can't be hurt by the spiders
	float		m_DamageCooldownTimerDefault	= 2.0f;
	float		m_DamageCooldownTimer			= m_DamageCooldownTimerDefault;

	// How fast the player should blink while being in the damage-cooldown state
	// The lower the value, the faster the player will blink
	float		m_BlinkingIntervalDefault		= 0.005f;
	float		m_BlinkingInterval				= m_BlinkingIntervalDefault;

	int32_t		m_HorizontalDirection			= EState::IDLE;
	int32_t		m_VerticalDirection				= EState::IDLE;

	SDL_FPoint	m_Velocity						= {0.0f, 0.0f};
	SDL_FPoint	m_HorizontalColliderOffset		= {0.0f, 0.0f};
	SDL_FPoint	m_VerticalColliderOffset		= {0.0f, 0.0f};

	SDL_FRect	m_HorizontalCollider			= {0.0f, 0.0f, 0.0f, 0.0f};
	SDL_FRect	m_VerticalCollider				= {0.0f, 0.0f, 0.0f, 0.0f};

	bool		m_IsRunning						= false;
	bool		m_IsJumping						= false;
	bool		m_IsAttacking					= false;
	bool		m_DamageCooldown				= false;
	bool		m_Show							= true;


};