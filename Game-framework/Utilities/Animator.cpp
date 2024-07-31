#include "Animator.h"

CAnimator::CAnimator(void)
: m_pTexture(nullptr)
, m_ClipQuad({0, 0, 0, 0})
, m_Speed(0.0f)
, m_Advance(0.0f)
, m_NumFrames(0)
, m_StartFrame(0)
, m_EndFrame(0)
, m_FrameWidth(0)
, m_FrameHeight(0)
, m_Row(0)
, m_CurrentFrame(0)
, m_Name("")
, m_Started(true)
, m_Loop(true)
, m_Direction(EDirection::FORWARD)
{

}

CAnimator::~CAnimator(void)
{

}

void CAnimator::Set(SDL_Texture* pTexture, const uint32_t NumFrames, const uint32_t StartFrame, const uint32_t EndFrame, const uint32_t Row, const SDL_FPoint& rFrameSize, const float Speed, const std::string& rName, const bool Loop, const EDirection Direction)
{
	m_pTexture		= pTexture;
	m_Speed			= Speed;
	m_NumFrames		= NumFrames;
	m_StartFrame	= StartFrame;
	m_EndFrame		= EndFrame;
	m_FrameWidth	= (uint32_t)rFrameSize.x;
	m_FrameHeight	= (uint32_t)rFrameSize.y;
	m_Row			= Row;
	m_CurrentFrame	= StartFrame;
	m_Name			= rName;
	m_Loop			= Loop;
	m_Direction		= Direction;

	Reset();
}

void CAnimator::Update(const float DeltaTime)
{
	if(!m_Started)
		return;

	m_Advance += m_Speed * DeltaTime;

	if(m_Advance > 1.0f)
	{
		const bool Forward = (m_Direction == EDirection::FORWARD);

		m_CurrentFrame += (Forward ? 1 : -1);

		if(Forward)
		{
			if(m_CurrentFrame > (int32_t)m_EndFrame)
			{
				m_CurrentFrame = (m_Loop ? m_StartFrame : m_EndFrame);

				if (m_pAnimationEndCallback)
					m_pAnimationEndCallback();
			}
		}

		else
		{
			if(m_CurrentFrame < (int32_t)m_EndFrame)
			{
				m_CurrentFrame = (m_Loop ? m_StartFrame : m_EndFrame);

				if (m_pAnimationEndCallback)
					m_pAnimationEndCallback();
			}
		}

		SetClipQuad();

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

	SetClipQuad();
}

void CAnimator::SetClipQuad(void)
{
	m_ClipQuad.x = m_FrameWidth		* m_CurrentFrame;
	m_ClipQuad.y = m_FrameHeight	* m_Row;
	m_ClipQuad.w = m_FrameWidth;
	m_ClipQuad.h = m_FrameHeight;
}