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

	virtual bool Create(const std::string& textureFileName, const SDL_FPoint& position) override;

};