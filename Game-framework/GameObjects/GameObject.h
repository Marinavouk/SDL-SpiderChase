#pragma once

#include <SDL.h>
#include <vector>

class CApplication;

class CGameObject
{
public:

	typedef std::vector<CGameObject*> GameObjectList;

public:

			 CGameObject(void)														{}
			 CGameObject(CApplication* application) : m_pApplication(application)	{}
	virtual ~CGameObject(void)														{}

	virtual bool	Create(const SDL_FPoint& position)																		= 0;
	virtual void	Destroy(void);
	virtual void	Update(const float deltaTime)																			{}
	virtual void	Render(void)																							{}
	virtual void	RenderDebug(void)																						{}
	virtual void	HandleInput(const float deltaTime)																		{}
	virtual void	HandleCollision(const GameObjectList& obstacles, const GameObjectList& enemies, const float deltaTime)	{}

public:

	SDL_FPoint		GetPosition(void) const						{return {m_Rectangle.x, m_Rectangle.y};}
	void			SetPosition(const SDL_FPoint newPosition)	{m_Rectangle = {newPosition.x, newPosition.y, m_Rectangle.w, m_Rectangle.h};}

	SDL_FRect&		GetCollider(void) const						{return (SDL_FRect&)m_Collider;}

protected:

	CApplication*	m_pApplication	= nullptr;

	SDL_Texture*	m_pTexture		= nullptr;

	SDL_FRect		m_Rectangle		= {0.0f, 0.0f, 0.0f, 0.0f};
	SDL_FRect		m_Collider		= {0.0f, 0.0f, 0.0f, 0.0f};

};