#include "Chair.h"

#include "Application.h"

bool CChair::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	if (!CGameObject::Create(textureFileName, position))
		return false;

	const SDL_FPoint textureSize	= m_pTexture->GetSize();
	const SDL_FPoint newTextureSize = {textureSize.x * 0.7f, textureSize.y * 0.7f};

	m_pTexture->SetSize(newTextureSize);

	m_Size = newTextureSize;

	m_Rectangle = {position.x, m_pApplication->GetWindow().GetSize().y - m_Size.y, m_Size.x, m_Size.y};

	m_Collider = {m_Rectangle.x + 5.0f, m_Rectangle.y + 85.0f, m_Size.x - 10.0f, 20.0f};

	return true;
}

void CChair::Render(void)
{
	m_pTexture->Render({m_Rectangle.x, m_pApplication->GetWindow().GetSize().y - m_Size.y});
}

void CChair::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider);
}