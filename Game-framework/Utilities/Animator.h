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

	 CAnimator							(void);
	~CAnimator							(void);

	void		Set						(SDL_Texture* pTexture, const uint32_t NumFrames, const uint32_t StartFrame, const uint32_t EndFrame, const uint32_t Row, const SDL_FPoint& rFrameSize, const float Speed, const std::string& rName, const bool Loop, const EDirection Direction);

	void		Update					(const float DeltaTime);

	void		Start					(void);
	void		Stop					(void);

	void		Reset					(void);

public:

	void		SetAnimationEndCallback	(AnimantionEndedCB pAnimationEndCallback) {m_pAnimationEndCallback = pAnimationEndCallback;}

	SDL_Rect&	GetClipQuad				(void) const		{return (SDL_Rect&)m_ClipQuad;}

	float		GetSpeed				(void) const		{return m_Speed;}
	void		SetSpeed				(const float Speed)	{m_Speed = Speed;}

	uint32_t	GetEndFrame				(void) const		{return m_EndFrame;}

	int32_t		GetCurrentFrame			(void) const		{return m_CurrentFrame;}

	SDL_FPoint	GetFrameSize			(void) const		{return {(float)m_FrameWidth, (float)m_FrameHeight};}

	bool		IsStarted				(void) const		{return m_Started;}

private:

	void		SetClipQuad				(void);

private:

	AnimantionEndedCB	m_pAnimationEndCallback;

	SDL_Texture*		m_pTexture;

	SDL_Rect			m_ClipQuad;

	float				m_Speed;
	float				m_Advance;

	uint32_t			m_NumFrames;
	uint32_t			m_StartFrame;
	uint32_t			m_EndFrame;
	uint32_t			m_FrameWidth;
	uint32_t			m_FrameHeight;
	uint32_t			m_Row;

	int32_t				m_CurrentFrame;

	std::string			m_Name;

	bool				m_Started;
	bool				m_Loop;

	EDirection			m_Direction;

};