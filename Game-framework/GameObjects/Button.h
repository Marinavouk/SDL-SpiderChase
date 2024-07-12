#pragma once

#include <SDL.h> 
#include <SDL_ttf.h>
#include <string>

class Application;
class InputHandler;

class Button
{
public:

	 Button(void) {}
	~Button(void) {}

	bool		Create(Application* application, TTF_Font* font, const std::string& text, const SDL_Color& textColor);
	void		Destroy(void);

	void		Update(InputHandler* inputHandler);
	void		Render(SDL_Renderer* renderer, const SDL_FPoint* mousePosition = nullptr);

	bool		IsPressed(InputHandler* inputHandler);

public:

	// Place getters and setters in their own public field

	SDL_FPoint&	GetPosition(void) const								{return (SDL_FPoint&)position;}
	void		SetPosition(const SDL_FPoint& newPosition);

	void		SetBackgroundColor(const SDL_Color& color)			{bgColor = color;}
	void		SetBackgroundPressedColor(const SDL_Color& color)	{bgColorPressed = color;}
	void		SetTextColor(const SDL_Color& color)				{txtColor = color;}
	void		SetTextColorHovered(const SDL_Color& color)			{txtColorHovered = color;}
	void		SetTextColorPressed(const SDL_Color& color)			{txtColorPressed = color;}

	void		SetTriggerButton(const int newTriggerButton)		{triggerButton = newTriggerButton;}

private:

	bool		PointInside(const SDL_FPoint& point);

private:

	SDL_Texture*	texture			= nullptr;

	SDL_FPoint		position		= {0.0f, 0.0f};

	SDL_FRect		textRect		= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_Color		bgColor			= {0,	0,		0,		255};
	SDL_Color		bgColorPressed	= {0,	0,		0,		255};
	SDL_Color		txtColor		= {255, 255,	255,	255};
	SDL_Color		txtColorHovered = {255, 255,	255,	255};
	SDL_Color		txtColorPressed	= {255, 255,	255,	255};

	float			scaleDefault	= 1.0f;
	float			scalePressed	= 1.1f;
	float			currentScale	= scaleDefault;

	int				triggerButton	= SDL_BUTTON_LEFT;

	bool			held			= false;

};