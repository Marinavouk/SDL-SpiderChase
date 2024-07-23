#include "Spider.h"

#include "Application.h"

bool CSpider::Create(void)
{
	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture("Assets/Textures/spiderTest.png");
	if (!m_pTexture)
		return false;

	int textureWidth = 0;
	int textureHeight = 0;
	SDL_QueryTexture(m_pTexture, nullptr, nullptr, &textureWidth, &textureHeight);

	m_Size = { (float)(textureWidth * 0.7f), (float)(textureHeight * 0.7f) };

	m_Rectangle = { 900.0f, m_pApplication->GetWindow().GetSize().y - m_Size.y, m_Size.x, m_Size.y };

	m_Collider = { m_Rectangle.x + 5.0f, m_Rectangle.y + 85.0f, m_Size.x - 10.0f, 20.0f };

	return true;
}

void CSpider::Render(void)
{
	m_pApplication->GetTextureHandler().RenderTexture(m_pTexture, { m_Rectangle.x, m_pApplication->GetWindow().GetSize().y - m_Size.y }, nullptr, &m_Size);
}

void CSpider::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider);
}
