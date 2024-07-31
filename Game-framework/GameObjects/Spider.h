#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Utilities/Animator.h"

#include <SDL.h>
#include <vector>

class CSpider final : public CGameObject
{
public:

	 CSpider(void)													{}
	 CSpider(CApplication* application) : CGameObject(application)	{}
	~CSpider(void)													{}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position) override;
	virtual void	Destroy(void);
	virtual void	Render(void) override;
	virtual void	RenderDebug(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;

public:

	void			SetTarget(CGameObject* target) {m_pTarget = target;}

private:

	bool			ResolveObstacleYCollision(const SDL_FRect& collider);
	void			SyncCollider(void);
	void			ActivateIdleAnimation(void);
	void			ActivateWalkingAnimation(void);

private:

	enum EState
	{
		MOVING_DOWN_FROM_CEILING = 0,
		HANGING_IN_THREAD,
		FALLING_DOWN,
		CHASING_PLAYER
	};

private:

	CGameObject*	m_pTarget			= nullptr;

	CAnimator*		m_pAnimatorIdle		= nullptr;
	CAnimator*		m_pAnimatorWalking	= nullptr;
	CAnimator*		m_pCurrentAnimator	= nullptr;

	float			m_Gravity			= 1500.0f;
	float			m_Scale				= 1.0f;
	float			m_LifeTime			= 0.0f;

	SDL_FPoint		m_StartPosition		= {0.0f, 0.0f};
	SDL_FPoint		m_Velocity			= {0.0f, 0.0f};
	SDL_FPoint		m_ColliderOffset	= {16.0f * m_Scale, 15.0f * m_Scale};

	EState			m_State				= EState::MOVING_DOWN_FROM_CEILING;

};