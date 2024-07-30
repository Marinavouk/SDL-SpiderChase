#include "GameObject.h"

#include "Application.h"

bool CGameObject::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture(textureFileName);

	int textureWidth	= 0;
	int textureHeight	= 0;
	SDL_QueryTexture(m_pTexture->GetTexture(), nullptr, nullptr, &textureWidth, &textureHeight);

	m_Rectangle = {position.x, position.y, (float)textureWidth, (float)textureHeight};

	m_Collider = m_Rectangle;

	return true;
}

void CGameObject::Destroy(void)
{
	m_pApplication->GetTextureHandler().DestroyTexture(m_pTexture->GetName());
	m_pTexture = nullptr;
}

void CGameObject::Render(void)
{
	m_pTexture->Render({m_Rectangle.x, m_Rectangle.y});
}

void CGameObject::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

//	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
//	SDL_RenderDrawRectF(renderer, &m_Rectangle);

	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider);
}