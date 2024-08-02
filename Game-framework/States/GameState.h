#pragma once

#include "GameObjects/GameObject.h"
#include "State.h"
#include "Utilities/Texture.h"
#include "Handlers/TextureHandler.h"

#include <SDL.h>
#include <SDL_mixer.h>

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

	void			OnPlayerAttach(void);

private:

	typedef std::vector<CGameObject*> GameObjectList;

private:

	// Declare the game objects here and then create/destroy them in the OnEnter- and OnExit functions

	CTexture*		m_pBackground		= nullptr;
	CTexture*		m_pHeartRed			= nullptr;
	CTexture*		m_pHeartBlack		= nullptr;

	Mix_Music*		m_pMusic			= nullptr;

	CGameObject*	m_pPlayer			= nullptr;
	CGameObject*	m_pTable			= nullptr;
	CGameObject*	m_pChair			= nullptr;
	CGameObject*	m_pSpider			= nullptr;

	int				m_VolumeLimiter		= 100;

	GameObjectList	m_Obstacles			= {};
	GameObjectList	m_Enemies			= {};

};