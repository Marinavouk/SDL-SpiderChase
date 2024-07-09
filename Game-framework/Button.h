#pragma once

#include <SDL.h> 
#include <SDL_ttf.h>
#include <string>

class Application;

class Button
{
public:

	 Button(void) {}
	~Button(void) {}

	bool		Create(Application* application, TTF_Font* font, const std::string& text, const SDL_FPoint& backgroundSize, const SDL_Color& backgroundColor, const SDL_Color& textColor, const SDL_Color& textColorHovered);
	void		Destroy(void);

	void		Render(SDL_Renderer* renderer, const SDL_FPoint& mousePosition);

	bool		PointInside(const SDL_FPoint& point);

public:

	// Place getters and setters in their own public field

	SDL_FPoint&	GetPosition(void) const						{return (SDL_FPoint&)position;}
	void		SetPosition(const SDL_FPoint& newPosition);

	SDL_FPoint	GetSize(void) const							{return (SDL_FPoint&)size;}

	void		SetBackgroundColor(const SDL_Color& color)	{bgColor = color;}
	void		SetTextColor(const SDL_Color& color)		{txtColor = color;}
	void		SetTextColorHovered(const SDL_Color& color)	{txtColorHovered = color;}

private:

	SDL_Texture*	texture			= nullptr;

	SDL_FPoint		position		= {0.0f, 0.0f};
	SDL_FPoint		size			= {0.0f, 0.0f};

	SDL_FRect		textRect		= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_Color		bgColor			= {100, 100,	100,	255};
	SDL_Color		txtColor		= {255, 255,	255,	255};
	SDL_Color		txtColorHovered = {255, 0,		0,		255};

};