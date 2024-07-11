#include "InputHandler.h"

void InputHandler::Update(void)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	for (size_t i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		previousKeyboardState[i]	= currentKeyboardState[i];
		currentKeyboardState[i]		= (keyboardState[i] == SDL_TRUE);
	}

	int				mouseX			= 0;
	int				mouseY			= 0;
	const Uint32	mouseState		= SDL_GetMouseState(&mouseX, &mouseY);	

	mousePosition = {(float)mouseX, (float)mouseY};

	for (size_t i = 0; i < 8; i++)
	{
		previousMouseState[i]	= currentMouseState[i];
		currentMouseState[i]	= ((mouseState & SDL_BUTTON(i)) ? true : false);
	}
}

bool InputHandler::KeyPressed(const SDL_Scancode keyboardKey)
{
	return (currentKeyboardState[keyboardKey] && !previousKeyboardState[keyboardKey]);
}

bool InputHandler::KeyHeld(const SDL_Scancode keyboardKey)
{
	return currentKeyboardState[keyboardKey];
}

bool InputHandler::KeyReleased(const SDL_Scancode keyboardKey)
{
	return (!currentKeyboardState[keyboardKey] && previousKeyboardState[keyboardKey]);
}

bool InputHandler::MouseButtonPressed(const int mouseButton)
{
	return (currentMouseState[mouseButton] && !previousMouseState[mouseButton]);
}

bool InputHandler::MouseButtonHeld(const int mouseButton)
{
	return currentMouseState[mouseButton];
}

bool InputHandler::MouseButtonReleased(const int mouseButton)
{
	return (!currentMouseState[mouseButton] && previousMouseState[mouseButton]);
}