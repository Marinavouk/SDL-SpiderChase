#include "Table.h"

#include "Application.h"

bool CTable::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	if (!CGameObject::Create(textureFileName, position))
		return false;

	const SDL_FPoint textureSize	= m_pTexture->GetSize();
	const SDL_FPoint newTextureSize = {textureSize.x * 0.7f, textureSize.y * 0.7f};

	m_pTexture->SetSize(newTextureSize);

	m_Rectangle = {position.x, m_pApplication->GetWindow().GetSize().y - newTextureSize.y, newTextureSize.x, newTextureSize.y};

	m_Collider = {m_Rectangle.x + 10.0f, m_Rectangle.y + 10.0f, newTextureSize.x - 20.0f, 25.0f};

	return true;
}