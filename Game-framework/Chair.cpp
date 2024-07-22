#include "Chair.h"

#include "Application.h"

bool CChair::Create(void)
{
	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture("Assets/Textures/chair.png");
	if (!m_pTexture)
		return false;

	int textureWidth = 0;
	int textureHeight = 0;
	SDL_QueryTexture(m_pTexture, nullptr, nullptr, &textureWidth, &textureHeight);

	m_Size = { (float)(textureWidth * 0.7f), (float)(textureHeight * 0.7f) };

	m_Rectangle = { 900.0f, m_pApplication->GetWindow().GetSize().y - m_Size.y, m_Size.x, m_Size.y };

	m_Collider = { m_Rectangle.x, m_Rectangle.y, m_Size.x, 32.0f };

	return true;
}

void CChair::Render(void)
{
	m_Collider = { m_Rectangle.x + 10.0f, m_Rectangle.y + 80.0f, m_Size.x - 20.0f, 25.0f };

	m_pApplication->GetTextureHandler().RenderTexture(m_pTexture, { m_Rectangle.x, m_pApplication->GetWindow().GetSize().y - m_Size.y }, nullptr, &m_Size);
}

void CChair::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider);
}