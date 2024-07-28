#include "Table.h"

#include "Application.h"

bool CTable::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	if (!CGameObject::Create(textureFileName, position))
		return false;

	const SDL_FPoint textureSize	= m_pTexture->GetSize();
	const SDL_FPoint newTextureSize = {textureSize.x * 0.7f, textureSize.y * 0.7f};

	m_pTexture->SetSize(newTextureSize);

	m_Size = newTextureSize;

	m_Rectangle = {position.x, m_pApplication->GetWindow().GetSize().y - m_Size.y, m_Size.x, m_Size.y};

	m_Collider = {m_Rectangle.x + 10.0f, m_Rectangle.y + 10.0f, m_Size.x - 20.0f, 25.0f};

	return true;
}

void CTable::Render(void)
{
	m_pTexture->Render({m_Rectangle.x, m_pApplication->GetWindow().GetSize().y - m_Size.y});
}

void CTable::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider);
}