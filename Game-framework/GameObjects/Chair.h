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

	virtual bool Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void SetPosition(const SDL_FPoint& position) override;

private:

	SDL_FPoint m_ColliderOffset = {5.0f, 85.0f};

};