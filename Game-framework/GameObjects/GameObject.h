#pragma once

#include "Utilities/Texture.h"

#include <SDL.h>
#include <string>
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

	virtual bool	Create(const std::string& textureFileName, const SDL_FPoint& position, const uint32_t maxHealth);
	virtual void	Destroy(void);
	virtual void	Render(void);
	virtual void	RenderDebug(void);
	virtual void	Update(const float deltaTime)													{}
	virtual void	HandleInput(const float deltaTime)												{}
	virtual void	HandleObstacleCollision(const GameObjectList& obstacles, const float deltaTime)	{}
	virtual void	HandleEnemyCollision(const GameObjectList& enemies, const float deltaTime)		{}

public:

	uint32_t		GetHealth(void) const							{return m_CurrentHealth;}

	SDL_FPoint		GetRectanglePosition(void) const				{return {m_Rectangle.x, m_Rectangle.y};}
	SDL_FPoint		GetRectangleCenterPosition(void) const			{return {m_Rectangle.x + (m_Rectangle.w * 0.5f), m_Rectangle.y + (m_Rectangle.h * 0.5f)};}
	SDL_FPoint		GetRectangleSize(void) const					{return {m_Rectangle.w, m_Rectangle.h};}
	void			SetRectanglePosition(const SDL_FPoint position)	{m_Rectangle.x = position.x; m_Rectangle.y = position.y;}

	SDL_FPoint		GetColliderPosition(void) const					{return {m_Collider.x, m_Collider.y};}
	SDL_FPoint		GetColliderCenterPosition(void) const			{return {m_Collider.x + (m_Collider.w * 0.5f), m_Collider.y + (m_Collider.h * 0.5f)};}
	SDL_FPoint		GetColliderSize(void) const						{return {m_Collider.w, m_Collider.h};}

	SDL_FRect&		GetRectangle(void) const						{return (SDL_FRect&)m_Rectangle;}
	SDL_FRect&		GetCollider(void) const							{return (SDL_FRect&)m_Collider;}


protected:

	CApplication*	m_pApplication	= nullptr;

	CTexture*		m_pTexture		= nullptr;

	uint32_t		m_CurrentHealth		= 3;

	SDL_FRect		m_Rectangle		= {0.0f, 0.0f, 0.0f, 0.0f};
	SDL_FRect		m_Collider		= {0.0f, 0.0f, 0.0f, 0.0f};

};