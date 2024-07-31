#pragma once

#include <SDL.h>
#include <functional>
#include <string>

typedef std::function<void()> AnimantionEndedCB;

class CAnimator
{
public:

	enum EDirection
	{
		FORWARD = 0,
		BACKWARD
	};

public:

	 CAnimator(void) {}
	~CAnimator(void) {}

	void		Set(const uint32_t numFrames, const uint32_t startFrame, const uint32_t endFrame, const uint32_t row, const SDL_FPoint& frameSize, const float speed, const bool loop, const EDirection direction);

	void		Update(const float deltaTime);

	void		Start(void);
	void		Stop(void);

	void		Reset(void);

public:

	void		SetAnimationEndCallback(AnimantionEndedCB animationEndCallback)	{m_pAnimationEndCallback = animationEndCallback;}

	SDL_Rect&	GetClipRectangle(void) const									{return (SDL_Rect&)m_ClipRectangle;}

	float		GetSpeed(void) const											{return m_Speed;}
	void		SetSpeed(const float speed)										{m_Speed = speed;}

	uint32_t	GetEndFrame(void) const											{return m_EndFrame;}

	int32_t		GetCurrentFrame(void) const										{return m_CurrentFrame;}

	SDL_FPoint	GetFrameSize(void) const										{return {(float)m_FrameWidth, (float)m_FrameHeight};}

	bool		IsStarted(void) const											{return m_Started;}

private:

	void		SetClipRectangle(void);

private:

	AnimantionEndedCB	m_pAnimationEndCallback	= nullptr;

	SDL_Rect			m_ClipRectangle			= {0, 0, 0, 0};

	float				m_Speed					= 0.0f;
	float				m_Advance				= 0.0f;

	uint32_t			m_NumFrames				= 0;
	uint32_t			m_StartFrame			= 0;
	uint32_t			m_EndFrame				= 0;
	uint32_t			m_FrameWidth			= 0;
	uint32_t			m_FrameHeight			= 0;
	uint32_t			m_Row					= 0;

	int32_t				m_CurrentFrame			= 0;

	bool				m_Started				= true;
	bool				m_Loop					= true;

	EDirection			m_Direction				= EDirection::FORWARD;

};