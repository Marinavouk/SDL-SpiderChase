#pragma once

#include "GameObjects/GameObject.h"

#include <SDL.h>
#include <vector>

class CChair final : public CGameObject
{
public:

	 CChair(void)													{}
	 CChair(CApplication* application) : CGameObject(application)	{}
	~CChair(void)													{}

	virtual bool Create(const SDL_FPoint& position) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;

private:

	SDL_FPoint m_Size = {0.0f, 0.0f};

};