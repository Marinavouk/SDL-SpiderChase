#include "Button.h"

#include "Application.h"

#include <algorithm> // Used by std::max
#include <iostream>
#include <vector>

bool Button::Create(Application* application, TTF_Font* font, const std::string& text, const SDL_FPoint& backgroundSize, const SDL_Color& backgroundColor, const SDL_Color& textColor, const SDL_Color& textColorHovered)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), { 255, 255, 255, 255 });
	if (!surface)
	{
		std::cout << "Error: failed to create surface" << std::endl;
		std::cout << SDL_GetError() << std::endl;

		return false;
	}

	texture = application->GetTextureHandler()->CreateTextureFromSurface(surface);

	SDL_FreeSurface(surface);

	if (!texture)
		return false;

	int textureWidth = 0;
	int textureHeight = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

	position = { 0.0f, 0.0f };
	size = { std::max(backgroundSize.x, (float)textureWidth), std::max(backgroundSize.y, (float)textureHeight) };
	textRect = { 0.0f, 0.0f, (float)textureWidth, (float)textureHeight };
	bgColor = backgroundColor;
	txtColor = textColor;
	txtColorHovered = textColorHovered;

	SetPosition(position);

	return true;
}

void Button::Destroy(void)
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void Button::Render(SDL_Renderer* renderer, const SDL_FPoint& mousePosition)
{
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRectF(renderer, &textRect);

	const SDL_Color textColor = (PointInside(mousePosition) ? txtColorHovered : txtColor);
	SDL_SetTextureColorMod(texture, textColor.r, textColor.g, textColor.b);

	SDL_RenderCopyF(renderer, texture, nullptr, &textRect);
}

bool Button::PointInside(const SDL_FPoint& point)
{
	return (SDL_PointInFRect(&point, &textRect) == SDL_TRUE);
}

void Button::SetPosition(const SDL_FPoint& newPosition)
{
	position = newPosition;

	textRect.x = position.x - (textRect.w * 0.5f);
	textRect.y = position.y - (textRect.h * 0.5f);
}