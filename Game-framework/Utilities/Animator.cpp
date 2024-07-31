#include "Animator.h"

void CAnimator::Set(const uint32_t numFrames, const uint32_t startFrame, const uint32_t endFrame, const uint32_t row, const SDL_FPoint& frameSize, const float speed, const bool loop, const EDirection direction)
{
	m_Speed			= speed;
	m_NumFrames		= numFrames;
	m_StartFrame	= startFrame;
	m_EndFrame		= endFrame;
	m_FrameWidth	= (uint32_t)frameSize.x;
	m_FrameHeight	= (uint32_t)frameSize.y;
	m_Row			= row;
	m_CurrentFrame	= startFrame;
	m_Loop			= loop;
	m_Direction		= direction;

	Reset();
}

void CAnimator::Update(const float deltaTime)
{
	if(!m_Started)
		return;

	m_Advance += m_Speed * deltaTime;

	if (m_Advance > 1.0f)
	{
		const bool Forward = (m_Direction == EDirection::FORWARD);

		m_CurrentFrame += (Forward ? 1 : -1);

		if (Forward)
		{
			if (m_CurrentFrame > (int32_t)m_EndFrame)
			{
				m_CurrentFrame = (m_Loop ? m_StartFrame : m_EndFrame);

				if (m_pAnimationEndCallback)
					m_pAnimationEndCallback();
			}
		}

		else
		{
			if (m_CurrentFrame < (int32_t)m_EndFrame)
			{
				m_CurrentFrame = (m_Loop ? m_StartFrame : m_EndFrame);

				if (m_pAnimationEndCallback)
					m_pAnimationEndCallback();
			}
		}

		SetClipRectangle();

		m_Advance = 0.0f;
	}
}

void CAnimator::Start(void)
{
	m_Started = true;

	Reset();
}

void CAnimator::Stop(void)
{
	m_Started = false;
}

void CAnimator::Reset(void)
{
	m_Advance = 0.0f;

	m_CurrentFrame = m_StartFrame;

	SetClipRectangle();
}

void CAnimator::SetClipRectangle(void)
{
	m_ClipRectangle.x = m_FrameWidth	* m_CurrentFrame;
	m_ClipRectangle.y = m_FrameHeight	* m_Row;
	m_ClipRectangle.w = m_FrameWidth;
	m_ClipRectangle.h = m_FrameHeight;
}