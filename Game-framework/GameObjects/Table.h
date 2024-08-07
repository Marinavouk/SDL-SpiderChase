#pragma once

#include "GameObject.h"

#include <SDL.h>
#include <vector>

class CTable final : public CGameObject
{
public:

	 CTable(void)													{}
	 CTable(CApplication* application) : CGameObject(application)	{}
	~CTable(void)													{}

	virtual bool Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void SetPosition(const SDL_FPoint& position) override;

private:

	SDL_FPoint m_ColliderOffset = {10.0f, 10.0f};

};