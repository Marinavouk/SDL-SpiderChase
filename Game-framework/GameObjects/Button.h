#pragma once

#include <SDL.h> 
#include <SDL_ttf.h>
#include <string>

class CApplication;
class CInputHandler;

class CButton
{
public:

	 CButton(void) {}
	~CButton(void) {}

	bool		Create(CApplication* application, TTF_Font* font, const std::string& text, const SDL_Color& textColor);
	void		Destroy(void);

	void		Update(CInputHandler& inputHandler);
	void		Render(SDL_Renderer* renderer, const SDL_FPoint* mousePosition = nullptr);

	bool		IsPressed(CInputHandler& inputHandler);

public:

	// Place getters and setters in their own public field

	SDL_FPoint&	GetPosition(void) const								{return (SDL_FPoint&)m_Position;}
	void		SetPosition(const SDL_FPoint& newPosition);

	void		SetBackgroundColor(const SDL_Color& color)			{m_BackgroundColor = color;}
	void		SetBackgroundPressedColor(const SDL_Color& color)	{m_BackgroundColorPressed = color;}
	void		SetTextColor(const SDL_Color& color)				{m_TextColor = color;}
	void		SetTextColorHovered(const SDL_Color& color)			{m_TextColorHovered = color;}
	void		SetTextColorPressed(const SDL_Color& color)			{m_TextColorPressed = color;}

	void		SetTriggerButton(const int newTriggerButton)		{m_TriggerButton = newTriggerButton;}

private:

	bool		PointInside(const SDL_FPoint& point);

private:

	SDL_Texture*	m_pTexture					= nullptr;

	SDL_FPoint		m_Position					= {0.0f, 0.0f};

	SDL_FRect		m_TextRectangle				= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_Color		m_BackgroundColor			= {0,	0,		0,		255};
	SDL_Color		m_BackgroundColorPressed	= {0,	0,		0,		255};
	SDL_Color		m_TextColor					= {255, 255,	255,	255};
	SDL_Color		m_TextColorHovered			= {255, 255,	255,	255};
	SDL_Color		m_TextColorPressed			= {255, 255,	255,	255};

	float			m_ScaleDefault				= 1.0f;
	float			m_ScalePressed				= 1.1f;
	float			m_CurrentScale				= m_ScaleDefault;

	int				m_TriggerButton				= SDL_BUTTON_LEFT;

	bool			m_Held						= false;

};