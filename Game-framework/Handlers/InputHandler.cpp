#include "Pch.h"
#include "InputHandler.h"

void CInputHandler::Update(void)
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

bool CInputHandler::KeyPressed(const SDL_Scancode keyboardKey)
{
	return (m_CurrentKeyboardState[keyboardKey] && !m_PreviousKeyboardState[keyboardKey]);
}

bool CInputHandler::KeyHeld(const SDL_Scancode keyboardKey)
{
	return m_CurrentKeyboardState[keyboardKey];
}

bool CInputHandler::KeyReleased(const SDL_Scancode keyboardKey)
{
	return (!m_CurrentKeyboardState[keyboardKey] && m_PreviousKeyboardState[keyboardKey]);
}

bool CInputHandler::MouseButtonPressed(const int32_t mouseButton)
{
	return (m_CurrentMouseState[mouseButton] && !m_previousMouseState[mouseButton]);
}

bool CInputHandler::MouseButtonHeld(const int32_t mouseButton)
{
	return m_CurrentMouseState[mouseButton];
}

bool CInputHandler::MouseButtonReleased(const int32_t mouseButton)
{
	return (!m_CurrentMouseState[mouseButton] && m_previousMouseState[mouseButton]);
}