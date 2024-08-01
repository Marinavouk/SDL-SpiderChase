#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"

#include <SDL.h>

class CLife final : public CGameObject
{

public:
	CLife(void) {}
	CLife(CApplication* application) : CGameObject(application) {}
	~CLife(void) {}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position) override;

};

