#pragma once

#include <SDL.h>

class InputHandler
{
public:

	 InputHandler(void) {}
	~InputHandler(void) {}

	void		Update(void);

	bool		KeyPressed(const SDL_Scancode keyboardKey);
	bool		KeyHeld(const SDL_Scancode keyboardKey);
	bool		KeyReleased(const SDL_Scancode keyboardKey);

	bool		MouseButtonPressed(const int mouseButton);
	bool		MouseButtonHeld(const int mouseButton);
	bool		MouseButtonReleased(const int mouseButton);

public:

	// Place getters and setters in their own public field

	SDL_FPoint&	GetMousePosition(void) const {return (SDL_FPoint&)m_MousePosition;}

private:

	SDL_FPoint	m_MousePosition								= {0.0f, 0.0f};

	bool		m_CurrentKeyboardState[SDL_NUM_SCANCODES]	= {false};
	bool		m_PreviousKeyboardState[SDL_NUM_SCANCODES]	= {false};
	bool		m_CurrentMouseState[8]						= {false};
	bool		m_previousMouseState[8]						= {false};

};