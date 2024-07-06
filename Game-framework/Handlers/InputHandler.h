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
	bool		MouseButtonReleased(const int mouseButton);

public:

	// Place getters and setters in their own public field

	SDL_FPoint&	GetMousePosition(void) const {return (SDL_FPoint&)mousePosition;}

private:

	SDL_FPoint	mousePosition								= {0.0f, 0.0f};

	bool		currentKeyboardState[SDL_NUM_SCANCODES]		= {false};
	bool		previousKeyboardState[SDL_NUM_SCANCODES]	= {false};
	bool		currentMouseState[8]						= {false};
	bool		previousMouseState[8]						= {false};

};