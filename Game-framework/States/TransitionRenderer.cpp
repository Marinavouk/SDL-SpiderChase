#include "TransitionRenderer.h"

#include "Application.h"

// Used for std::clamp
#include <algorithm>

void TransitionRenderer::Create(Application* mainApplication, const SDL_FPoint& size)
{
	application = mainApplication;

	renderer = application->GetWindow()->GetRenderer();

	rect = {0.0f, 0.0f, size.x, size.y};
}

void TransitionRenderer::Destroy(void)
{
	renderer = nullptr;
	application = nullptr;
}

void TransitionRenderer::Update(const float deltaTime)
{
	if (state == EState::IDLE)
		return;

	const float delta = std::min(deltaTime, 0.0333f);

	if (state == EState::FADING_IN)
	{
		transitionValue = std::clamp(transitionValue - (fadeSpeed * delta), 0.0f, 1.0f);

		if(transitionValue <= 0.0f)
			state = EState::IDLE;
	}

	else if(state == EState::FADING_OUT)
	{
		transitionValue = std::clamp(transitionValue + (fadeSpeed * delta), 0.0f, 1.0f);

		if (transitionValue >= 1.0f)
		{
			transitionDelay -= deltaTime;

			if (transitionDelay <= 0.0f)
			{
				transitionDelay = 0.0f;

				application->OnTransitionOpaque();

				state = EState::FADING_IN;
			}
		}
	}
}

void TransitionRenderer::Render(void)
{
	if (state == EState::IDLE)
		return;

	SDL_Renderer* renderer = application->GetWindow()->GetRenderer();

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, (Uint8)(transitionValue * 255.0f));
	SDL_RenderFillRectF(renderer, &rect);
}

void TransitionRenderer::StartTransition(void)
{
	transitionValue = 0.0f;

	transitionDelay = transitionDelayDefault;

	state = EState::FADING_OUT;
}