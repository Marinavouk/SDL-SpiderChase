#pragma once

#include "GameObjects/GameObject.h"

#include <SDL.h>

class CFireball final : public CGameObject
{
public:
	 bool GetIsActive() const  { return m_IsActive; }

	 void SetIsActive(const bool isActive)  { m_IsActive = isActive; }

	 virtual void	Update(const float deltaTime);

private:

	bool m_IsActive = false;
};

