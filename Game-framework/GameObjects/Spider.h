#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Utilities/Animator.h"

class CSpider final : public CGameObject
{
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

	void			Activate(const SDL_FPoint& spawnPosition);

public:

	bool			GetIsActive(void) const				{return m_IsActive;}
	void			SetIsActive(const bool isActive)	{m_IsActive = isActive;}

	void			SetTarget(CGameObject* target)		{m_pTarget = target;}

private:

	bool			ResolveObstacleYCollision(const SDL_FRect& collider);
	void			SyncCollider(void);
	void			ActivateAnimator(CAnimator* animator);

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

	CGameObject*		m_pTarget				= nullptr;

	CAnimator*			m_pAnimatorHanging		= nullptr;
	CAnimator*			m_pAnimatorIdle			= nullptr;
	CAnimator*			m_pAnimatorWalking		= nullptr;
	CAnimator*			m_pCurrentAnimator		= nullptr;
	CAnimator*			m_pAnimatorDying		= nullptr;

	float				m_Gravity				= 1500.0f;
	float				m_Scale					= 1.2f;
	float				m_LifeTime				= 0.0f;
	float				m_Angle					= 0.0f;
	float				m_ThreadMoveVelocity	= 100.0f;
	float				m_MaxFallVelocity		= 500.0f;

	bool				m_IsActive				= false;

	SDL_FPoint			m_StartPosition			= {0.0f, 0.0f};
	SDL_FPoint			m_Velocity				= {130.0f, 0.0f};
	SDL_FPoint			m_ColliderOffset		= {16.0f * m_Scale, 15.0f * m_Scale};

	SDL_RendererFlip	m_FlipMethod			= SDL_RendererFlip::SDL_FLIP_NONE;

	EState				m_State					= EState::MOVING_DOWN_FROM_CEILING;

};