#include "Button.h"

#include "Application.h"

#include <algorithm> // Used by std::max
#include <iostream>
#include <vector>

bool Button::Create(Application* application, TTF_Font* font, const std::string& text, const SDL_Color& textColor)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255, 255});
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

	SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_SetTextureColorMod(texture, textColor.r, textColor.g, textColor.b);

	int textureWidth	= 0;
	int textureHeight	= 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

	position = {0.0f, 0.0f};
	textRect = {0.0f, 0.0f, (float)textureWidth, (float)textureHeight};
	txtColor = textColor;

	SetPosition(position);

	return true;
}

void Button::Destroy(void)
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void Button::Update(InputHandler* inputHandler)
{
	held = (PointInside(inputHandler->GetMousePosition()) && inputHandler->MouseButtonHeld(triggerButton));

	currentScale = (held ? scalePressed : scaleDefault);
}

void Button::Render(SDL_Renderer* renderer, const SDL_FPoint* mousePosition)
{
	if (held)
		SDL_SetRenderDrawColor(renderer, bgColorPressed.r, bgColorPressed.g, bgColorPressed.b, bgColorPressed.a);

	else
		SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);

	if (held)
	{
		const float		diffW	= ((textRect.w * scalePressed) - (textRect.w * scaleDefault)) * 0.5f;
		const float		diffH	= ((textRect.h * scalePressed) - (textRect.h * scaleDefault)) * 0.5f;
		const SDL_FRect dstRect	= {	textRect.x - diffW,
									textRect.y - diffH,
									textRect.w * scalePressed,
									textRect.h * scalePressed};

		SDL_RenderFillRectF(renderer, &dstRect);
	}

	else
		SDL_RenderFillRectF(renderer, &textRect);

	if (mousePosition)
	{
		SDL_Color textColor = (PointInside(*mousePosition) ? txtColorHovered : txtColor);
		SDL_SetTextureColorMod(texture, textColor.r, textColor.g, textColor.b);
	}

	if (held)
	{
		SDL_SetTextureColorMod(texture, txtColorPressed.r, txtColorPressed.g, txtColorPressed.b);

		const float		diffW	= ((textRect.w * scalePressed) - (textRect.w * scaleDefault)) * 0.5f;
		const float		diffH	= ((textRect.h * scalePressed) - (textRect.h * scaleDefault)) * 0.5f;
		const SDL_FRect dstRect	= {	textRect.x - diffW,
									textRect.y - diffH,
									textRect.w * scalePressed,
									textRect.h * scalePressed};

		SDL_RenderCopyF(renderer, texture, nullptr, &dstRect);
	}

	else
		SDL_RenderCopyF(renderer, texture, nullptr, &textRect);
}

bool Button::IsPressed(InputHandler* inputHandler)
{
	return (PointInside(inputHandler->GetMousePosition()) && inputHandler->MouseButtonReleased(triggerButton));
}

bool Button::PointInside(const SDL_FPoint& point)
{
	if (held)
	{
		const float		diffW	= ((textRect.w * scalePressed) - (textRect.w * scaleDefault)) * 0.5f;
		const float		diffH	= ((textRect.h * scalePressed) - (textRect.h * scaleDefault)) * 0.5f;
		const SDL_FRect dstRect	= {	textRect.x - diffW,
									textRect.y - diffH,
									textRect.w * scalePressed,
									textRect.h * scalePressed};

		return (SDL_PointInFRect(&point, &dstRect) == SDL_TRUE);
	}

	else
		return (SDL_PointInFRect(&point, &textRect) == SDL_TRUE);
}

void Button::SetPosition(const SDL_FPoint& newPosition)
{
	position = newPosition;

	textRect.x = position.x - (textRect.w * 0.5f);
	textRect.y = position.y - (textRect.h * 0.5f);
}