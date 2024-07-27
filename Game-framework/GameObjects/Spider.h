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

	virtual bool Create(void) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;

private:

	//////////////////////////////////////////////////////////////////////////
	// Temporary data
	float		m_Scale				= 1.0f;
	SDL_FPoint	m_ColliderOffset	= {16.0f * m_Scale, 15.0f * m_Scale};
	int			m_Direction			= 1;
	//////////////////////////////////////////////////////////////////////////

};