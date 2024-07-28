#pragma once

#include "GameObjects/GameObject.h"

#include <SDL.h>
#include <vector>

class CSpider final : public CGameObject
{
public:

	 CSpider(void)													{}
	 CSpider(CApplication* application) : CGameObject(application)	{}
	~CSpider(void)													{}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position) override;
	virtual void	Destroy(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	Render(void) override;
	virtual void	RenderDebug(void) override;

public:

	void			SetTarget(CGameObject* target) {m_pTarget = target;}

private:

	enum EState
	{
		HANGING_FROM_CEILING = 0,
		FALLING_DOWN,
		CHASING_PLAYER
	};

private:

	CGameObject*	m_pTarget	= nullptr;

	EState			m_State		= EState::HANGING_FROM_CEILING;

	//////////////////////////////////////////////////////////////////////////
	// Temporary data
	float		m_Scale				= 1.0f;
	SDL_FPoint	m_ColliderOffset	= {16.0f * m_Scale, 15.0f * m_Scale};
	int			m_Direction			= 1;
	//////////////////////////////////////////////////////////////////////////

};