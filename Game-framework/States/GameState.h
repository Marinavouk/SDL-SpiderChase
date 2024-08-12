#pragma once

#include "GameObjects/GameObject.h"
#include "State.h"
#include "Utilities/Texture.h"
#include "Handlers/TextureHandler.h"

class CGameState final : public CState
{
public:

	 CGameState(void)												{}
	 CGameState(CApplication* application) : CState(application)	{}
	~CGameState(void)												{}

	virtual bool	OnEnter(void) override;
	virtual void	OnExit(void) override;
	virtual void	Update(const float deltaTime) override;
	virtual void	Render(void) override;
	virtual void	RenderDebug(void) override;

private:

	void			OnPlayerAttacking(void);
	void			OnPlayerDying(void);

private:

	typedef std::vector<CGameObject*> GameObjectList;

private:

	// Declare the game objects here and then create/destroy them in the OnEnter- and OnExit functions

	CTexture*		m_pBackground			= nullptr;
	CTexture*		m_pHeartRed				= nullptr;
	CTexture*		m_pHeartBlack			= nullptr;

	TTF_Font*		m_pFont					= nullptr;

	Mix_Music*		m_pMusic				= nullptr;

	CGameObject*	m_pPlayer				= nullptr;
	CGameObject*	m_pTable				= nullptr;
	CGameObject*	m_pChair				= nullptr;
	CGameObject*	m_pSpider				= nullptr;

	int32_t			m_VolumeLimiter			= 100;

	uint32_t		m_SpiderCount			= 0;

	float			m_Timer					= 60.0f;

	// When the player has died, the game waits this long before fading out and changing to the end-of-round state
	float			m_DeathFadeDelayDefault	= 3.0f;
	float			m_DeathFadeDelay		= m_DeathFadeDelayDefault;

	bool			m_DeathFadeout			= false;

	GameObjectList	m_Obstacles				= {};
	GameObjectList	m_Enemies				= {};
	GameObjectList	m_FireballPool			= {};
	GameObjectList	m_ActiveFireballs		= {};

};