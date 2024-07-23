#pragma once

#include "GameObjects/GameObject.h"

#include <SDL.h>
#include <vector>

class CSpider final : public CGameObject
{
public:

	CSpider(void) {}
	CSpider(CApplication* application) : CGameObject(application) {}
		~CSpider(void) {}

	virtual bool Create(void) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;
	void Update(float deltaTime);

private:

	SDL_FPoint m_Size = { 0.0f, 0.0f };

	float Distance(float x1, float y1, float x2, float y2);
};

