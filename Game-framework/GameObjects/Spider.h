#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Utilities/Animator.h"


class CSpider final : public CGameObject
{
public:

	typedef std::function<void(const uint32_t index)> Callback;

public:

	 CSpider(void)													{}
	 CSpider(CApplication* application) : CGameObject(application)	{}
	~CSpider(void)													{}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void	Destroy(void);
	virtual void	Kill(void) override;
	virtual void	Render(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;

public:

	void			Activate(const SDL_FPoint& spawnPosition, const float threadLength, const uint32_t index);

public:

	void			SetDyingCallback(Callback dyingCallback)	{m_pDyingCallback = dyingCallback;}

	void			SetTarget(CGameObject* target)				{m_pTarget = target;}

	void			SetIndex(const uint32_t index)				{m_Index = index;}

	bool			GetIsActive(void) const						{return m_IsActive;}
	void			SetIsActive(const bool isActive)			{m_IsActive = isActive;}

private:

	bool			ResolveObstacleYCollision(const SDL_FRect& collider);
	void			SyncCollider(void);
	void			ActivateAnimator(CAnimator* animator);
	void			OnDyingAnimationEnd(void);

private:

	enum EState
	{
		MOVING_DOWN_FROM_CEILING = 0,
		HANGING_IN_THREAD,
		FALLING_DOWN,
		CHASING_PLAYER,
		DEAD
	};

private:

	Callback			m_pDyingCallback		= nullptr;

	CGameObject*		m_pTarget				= nullptr;

	CAnimator*			m_pAnimatorHanging		= nullptr;
	CAnimator*			m_pAnimatorIdle			= nullptr;
	CAnimator*			m_pAnimatorWalking		= nullptr;
	CAnimator*			m_pCurrentAnimator		= nullptr;
	CAnimator*			m_pAnimatorDying		= nullptr;

	float				m_Gravity				= 1500.0f;
	float				m_Scale					= 1.2f;
	float				m_Angle					= 0.0f;
	float				m_LifeTime				= 0.0f;
	float				m_ThreadLength			= 150.0f;
	float				m_ThreadMoveVelocity	= 100.0f;
	float				m_ChaseVelocity			= 130.0f;
	float				m_MaxFallVelocity		= 500.0f;

	uint32_t			m_Index					= 0;

	bool				m_IsActive				= false;

	SDL_FPoint			m_StartPosition			= {0.0f, 0.0f};
	SDL_FPoint			m_Velocity				= {0.0f, 0.0f};
	SDL_FPoint			m_ColliderOffset		= {16.0f * m_Scale, 15.0f * m_Scale};

	SDL_RendererFlip	m_FlipMethod			= SDL_RendererFlip::SDL_FLIP_NONE;

	EState				m_State					= EState::MOVING_DOWN_FROM_CEILING;

};