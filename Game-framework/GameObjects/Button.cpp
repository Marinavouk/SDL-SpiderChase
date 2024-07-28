#include "Button.h"

#include "Application.h"

#include <iostream>
#include <vector>

bool CButton::Create(CApplication* application, TTF_Font* font, const std::string& text, const SDL_Color& textColor)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255, 255});
	if (!surface)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create surface" << std::endl;
		std::cout << TTF_GetError() << std::endl;
	#endif
		return false;
	}

	m_pTexture = application->GetTextureHandler().CreateTextureFromSurface(surface, text);
	m_pTexture->SetColorMod(textColor);

	const SDL_FPoint textureSize = m_pTexture->GetSize();

	m_Position		= {0.0f, 0.0f};
	m_TextRectangle	= {0.0f, 0.0f, textureSize.x, textureSize.y};
	m_TextColor		= textColor;

	SetPosition(m_Position);

	return true;
}

void CButton::Destroy(CApplication* application)
{
	application->GetTextureHandler().DestroyTexture(m_pTexture->GetName());
	m_pTexture = nullptr;
}

void CButton::Update(CInputHandler& inputHandler)
{
	m_Held = (PointInside(inputHandler.GetMousePosition()) && inputHandler.MouseButtonHeld(m_TriggerButton));

	m_CurrentScale = (m_Held ? m_ScalePressed : m_ScaleDefault);
}

void CButton::Render(SDL_Renderer* renderer, const SDL_FPoint* mousePosition)
{
	if (m_Held)
		SDL_SetRenderDrawColor(renderer, m_BackgroundColorPressed.r, m_BackgroundColorPressed.g, m_BackgroundColorPressed.b, m_BackgroundColorPressed.a);

	else
		SDL_SetRenderDrawColor(renderer, m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);

	if (m_Held)
	{
		const float		diffW	= ((m_TextRectangle.w * m_ScalePressed) - (m_TextRectangle.w * m_ScaleDefault)) * 0.5f;
		const float		diffH	= ((m_TextRectangle.h * m_ScalePressed) - (m_TextRectangle.h * m_ScaleDefault)) * 0.5f;
		const SDL_FRect dstRect	= {	m_TextRectangle.x - diffW,
									m_TextRectangle.y - diffH,
									m_TextRectangle.w * m_ScalePressed,
									m_TextRectangle.h * m_ScalePressed};

		SDL_RenderFillRectF(renderer, &dstRect);
	}

	else
		SDL_RenderFillRectF(renderer, &m_TextRectangle);

	if (mousePosition)
		m_pTexture->SetColorMod((PointInside(*mousePosition) ? m_TextColorHovered : m_TextColor));

	if (m_Held)
	{
		m_pTexture->SetColorMod(m_TextColorPressed);

		const float		diffW	= ((m_TextRectangle.w * m_ScalePressed) - (m_TextRectangle.w * m_ScaleDefault)) * 0.5f;
		const float		diffH	= ((m_TextRectangle.h * m_ScalePressed) - (m_TextRectangle.h * m_ScaleDefault)) * 0.5f;
		const SDL_FRect dstRect	= {m_TextRectangle.x - diffW, m_TextRectangle.y - diffH, m_TextRectangle.w * m_ScalePressed, m_TextRectangle.h * m_ScalePressed};

		m_pTexture->SetSize({dstRect.w, dstRect.h});
		m_pTexture->Render({dstRect.x, dstRect.y});
	}

	else
	{
		m_pTexture->SetSize({m_TextRectangle.w, m_TextRectangle.h});
		m_pTexture->Render({m_TextRectangle.x, m_TextRectangle.y});
	}
}

bool CButton::IsPressed(CInputHandler& inputHandler)
{
	return (PointInside(inputHandler.GetMousePosition()) && inputHandler.MouseButtonReleased(m_TriggerButton));
}

bool CButton::PointInside(const SDL_FPoint& point)
{
	if (m_Held)
	{
		const float		diffW	= ((m_TextRectangle.w * m_ScalePressed) - (m_TextRectangle.w * m_ScaleDefault)) * 0.5f;
		const float		diffH	= ((m_TextRectangle.h * m_ScalePressed) - (m_TextRectangle.h * m_ScaleDefault)) * 0.5f;
		const SDL_FRect dstRect	= {m_TextRectangle.x - diffW, m_TextRectangle.y - diffH, m_TextRectangle.w * m_ScalePressed, m_TextRectangle.h * m_ScalePressed};

		return (SDL_PointInFRect(&point, &dstRect) == SDL_TRUE);
	}

	else
		return (SDL_PointInFRect(&point, &m_TextRectangle) == SDL_TRUE);
}

void CButton::SetPosition(const SDL_FPoint& newPosition)
{
	m_Position = newPosition;

	m_TextRectangle.x = m_Position.x - (m_TextRectangle.w * 0.5f);
	m_TextRectangle.y = m_Position.y - (m_TextRectangle.h * 0.5f);
}