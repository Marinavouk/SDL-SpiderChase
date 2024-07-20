#include "InputHandler.h"

void InputHandler::Update(void)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

	for (size_t i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		m_PreviousKeyboardState[i]	= m_CurrentKeyboardState[i];
		m_CurrentKeyboardState[i]		= (keyboardState[i] == SDL_TRUE);
	}

	int				mouseX		= 0;
	int				mouseY		= 0;
	const Uint32	mouseState	= SDL_GetMouseState(&mouseX, &mouseY);	

	m_MousePosition = {(float)mouseX, (float)mouseY};

	for (size_t i = 0; i < 8; i++)
	{
		m_previousMouseState[i]	= m_CurrentMouseState[i];
		m_CurrentMouseState[i]	= ((mouseState & SDL_BUTTON(i)) ? true : false);
	}
}

bool InputHandler::KeyPressed(const SDL_Scancode keyboardKey)
{
	return (m_CurrentKeyboardState[keyboardKey] && !m_PreviousKeyboardState[keyboardKey]);
}

bool InputHandler::KeyHeld(const SDL_Scancode keyboardKey)
{
	return m_CurrentKeyboardState[keyboardKey];
}

bool InputHandler::KeyReleased(const SDL_Scancode keyboardKey)
{
	return (!m_CurrentKeyboardState[keyboardKey] && m_PreviousKeyboardState[keyboardKey]);
}

bool InputHandler::MouseButtonPressed(const int mouseButton)
{
	return (m_CurrentMouseState[mouseButton] && !m_previousMouseState[mouseButton]);
}

bool InputHandler::MouseButtonHeld(const int mouseButton)
{
	return m_CurrentMouseState[mouseButton];
}

bool InputHandler::MouseButtonReleased(const int mouseButton)
{
	return (!m_CurrentMouseState[mouseButton] && m_previousMouseState[mouseButton]);
}