#include "Spider.h"

#include "Application.h"

bool CSpider::Create(const std::string& textureFileName, const SDL_FPoint& position)
{
	if (!CGameObject::Create(textureFileName, position))
		return false;

	m_pTexture = m_pApplication->GetTextureHandler().CreateTexture("Assets/Textures/spider.png");
	if (!m_pTexture)
		return false;

	m_Rectangle = {position.x, m_pApplication->GetWindow().GetSize().y - ((64.0f - m_ColliderOffset.y) * m_Scale), 64.0f * m_Scale, 64.0f * m_Scale};

	m_Collider = {m_Rectangle.x + m_ColliderOffset.x, m_Rectangle.y + m_ColliderOffset.y, 34.0f * m_Scale, 36.0f * m_Scale};

	return true;
}

void CSpider::Destroy(void)
{
	m_pTarget = nullptr;

	CGameObject::Destroy();
}

void CSpider::Update(const float deltaTime)
{
	/*
	m_Rectangle.x += ((m_Direction == 1) ? 120.0f : -120.0f) * deltaTime;

	const SDL_FPoint windowSize = m_pApplication->GetWindow().GetSize();

	if (m_Collider.x < 0.0f)
	{
		m_Rectangle.x = -m_ColliderOffset.x;

		m_Direction = 1;
	}

	else if(m_Collider.x > (windowSize.x - m_Collider.w))
	{
		const float rightOffset = m_Rectangle.w - (m_Collider.w + m_ColliderOffset.x);

		m_Rectangle.x = windowSize.x - (m_Rectangle.w - rightOffset);

		m_Direction = 0;
	}
	*/

	switch (m_State)
	{
		case EState::HANGING_FROM_CEILING:
		{
			break;
		}

		case EState::FALLING_DOWN:
		{
			break;
		}

		case EState::CHASING_PLAYER:
		{
			break;
		}

		default:
			break;
	}

	m_Collider.x = m_Rectangle.x + m_ColliderOffset.x;
	m_Collider.y = m_Rectangle.y + m_ColliderOffset.y;
}

void CSpider::Render(void)
{
	// Very temporary, will be removed when the spider is animated
	// This is just to have anything rendered to the screen
	const SDL_Rect		clipRect	= {256, 192, 64, 64};
	const SDL_FPoint	size		= {64.0f * m_Scale, 64.0f * m_Scale};
	m_pApplication->GetTextureHandler().RenderTexture(m_pTexture, {m_Rectangle.x, m_Rectangle.y}, &clipRect, &size);
}

void CSpider::RenderDebug(void)
{
	SDL_Renderer* renderer = m_pApplication->GetWindow().GetRenderer();

	SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Rectangle);

	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
	SDL_RenderDrawRectF(renderer, &m_Collider); 
}