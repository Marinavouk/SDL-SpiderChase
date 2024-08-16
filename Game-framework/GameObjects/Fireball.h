#pragma once

#include "GameObjects/GameObject.h"

class CFireball final : public CGameObject
{
public:

	 CFireball(void)													{}
	 CFireball(CApplication* application) : CGameObject(application)	{}
	~CFireball(void)													{}

	virtual void	Kill(void) override;
	virtual void	Render(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime) override;

public:

	void			Activate(const SDL_FPoint& spawnPosition, const SDL_RendererFlip flipMethod);

public:

	bool			GetIsActive(void) const				{return m_IsActive;}
	void			SetIsActive(const bool isActive)	{m_IsActive = isActive;}

private:

	SDL_FPoint			m_StartPosition	= {0.0f, 0.0f};

	SDL_RendererFlip	m_FlipMethod	= SDL_RendererFlip::SDL_FLIP_NONE;

	float				m_Velocity		= 500.0f;
	float				m_Angle			= 0.0f;
	float				m_LifeTime		= 0.0f;

	bool				m_IsActive		= false;

};