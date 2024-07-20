#include "TransitionRenderer.h"

#include "Application.h"

// Used for std::clamp
#include <algorithm>

TransitionRenderer::TransitionRenderer(void)
{

}

TransitionRenderer::TransitionRenderer(Application* application, const SDL_FPoint& size)
{
	application = application;

	m_pRenderer = application->GetWindow().GetRenderer();

	m_Transition = {0.0f, 0.0f, size.x, size.y};
}

TransitionRenderer::~TransitionRenderer(void)
{
	m_pRenderer		= nullptr;
	m_pApplication	= nullptr;
}

void TransitionRenderer::Update(const float deltaTime)
{
	if (m_State == EState::IDLE)
		return;

	if (m_State == EState::FADING_IN)
	{
		m_TransitionValue = std::clamp(m_TransitionValue - (m_FadeSpeed * std::min(deltaTime, 0.0333f)), 0.0f, 1.0f);

		if(m_TransitionValue <= 0.0f)
			m_State = EState::IDLE;
	}

	else if(m_State == EState::FADING_OUT)
	{
		m_TransitionValue = std::clamp(m_TransitionValue + (m_FadeSpeed * std::min(deltaTime, 0.0333f)), 0.0f, 1.0f);

		if (m_TransitionValue >= 1.0f)
		{
			m_TransitionDelay -= deltaTime;

			if (m_TransitionDelay <= 0.0f)
			{
				m_TransitionDelay = 0.0f;

				m_pApplication->OnTransitionOpaque();

				m_State = EState::FADING_IN;
			}
		}
	}
}

void TransitionRenderer::Render(void)
{
	if (m_State == EState::IDLE)
		return;

	SDL_SetRenderDrawColor(m_pRenderer, m_Color.r, m_Color.g, m_Color.b, (Uint8)(m_TransitionValue * 255.0f));
	SDL_RenderFillRectF(m_pRenderer, &m_Transition);
}

void TransitionRenderer::StartTransition(void)
{
	m_TransitionValue = 0.0f;

	m_TransitionDelay = m_TransitionDelayDefault;

	m_State = EState::FADING_OUT;
}