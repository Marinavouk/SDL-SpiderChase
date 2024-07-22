#include "Button.h"

#include "Application.h"

#include <algorithm> // Used by std::max
#include <iostream>
#include <vector>

bool CButton::Create(CApplication* application, TTF_Font* font, const std::string& text, const SDL_Color& textColor)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255, 255});
	if (!surface)
	{
	#if defined(_DEBUG)
		std::cout << "Error: failed to create surface" << std::endl;
		std::cout << SDL_GetError() << std::endl;
	#endif
		return false;
	}

	m_pTexture = application->GetTextureHandler().CreateTextureFromSurface(surface);

	SDL_FreeSurface(surface);

	if (!m_pTexture)
		return false;

	SDL_SetTextureBlendMode(m_pTexture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_SetTextureColorMod(m_pTexture, textColor.r, textColor.g, textColor.b);

	int textureWidth	= 0;
	int textureHeight	= 0;
	SDL_QueryTexture(m_pTexture, nullptr, nullptr, &textureWidth, &textureHeight);

	m_Position		= {0.0f, 0.0f};
	m_TextRectangle	= {0.0f, 0.0f, (float)textureWidth, (float)textureHeight};
	m_TextColor		= textColor;

	SetPosition(m_Position);

	return true;
}

void CButton::Destroy(void)
{
	SDL_DestroyTexture(m_pTexture);
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
	{
		SDL_Color textColor = (PointInside(*mousePosition) ? m_TextColorHovered : m_TextColor);
		SDL_SetTextureColorMod(m_pTexture, textColor.r, textColor.g, textColor.b);
	}

	if (m_Held)
	{
		SDL_SetTextureColorMod(m_pTexture, m_TextColorPressed.r, m_TextColorPressed.g, m_TextColorPressed.b);

		const float		diffW	= ((m_TextRectangle.w * m_ScalePressed) - (m_TextRectangle.w * m_ScaleDefault)) * 0.5f;
		const float		diffH	= ((m_TextRectangle.h * m_ScalePressed) - (m_TextRectangle.h * m_ScaleDefault)) * 0.5f;
		const SDL_FRect dstRect	= {m_TextRectangle.x - diffW, m_TextRectangle.y - diffH, m_TextRectangle.w * m_ScalePressed, m_TextRectangle.h * m_ScalePressed};

		SDL_RenderCopyF(renderer, m_pTexture, nullptr, &dstRect);
	}

	else
		SDL_RenderCopyF(renderer, m_pTexture, nullptr, &m_TextRectangle);
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