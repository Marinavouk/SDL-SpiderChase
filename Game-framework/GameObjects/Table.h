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

	virtual bool Create(const std::string& textureFileName, const SDL_FPoint& position) override;
	virtual void Render(void) override;
	virtual void RenderDebug(void) override;

private:

	SDL_FPoint m_Size = {0.0f, 0.0f};

};