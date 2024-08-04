#pragma once

#include "GameObjects/GameObject.h"

class CFireball final : public CGameObject
{
public:

	 CFireball(void) {}
	~CFireball(void) {}

	virtual void	Update(const float deltaTime);

public:

	bool			GetIsActive(void) const				{return m_IsActive;}
	void			SetIsActive(const bool isActive)	{m_IsActive = isActive;}

private:

	bool m_IsActive = false;

};