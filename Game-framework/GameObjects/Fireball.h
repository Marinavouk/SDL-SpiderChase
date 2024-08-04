#pragma once

#include "GameObjects/GameObject.h"

class CFireball final : public CGameObject
{
public:

	 CFireball(void)													{}
	 CFireball(CApplication* application) : CGameObject(application)	{}
	~CFireball(void)													{}

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth) override;
	virtual void	Update(const float deltaTime) override;

public:

	void			Activate(const SDL_FPoint& spawnPosition);

public:

	bool			GetIsActive(void) const				{return m_IsActive;}
	void			SetIsActive(const bool isActive)	{m_IsActive = isActive;}

private:

	bool m_IsActive = false;

};