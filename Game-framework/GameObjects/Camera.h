#pragma once

#include <SDL.h>

class Player;

class Camera
{
public:

	 Camera(void) {}
	~Camera(void) {}

	void		Create(const SDL_FPoint& startPosition, const SDL_FPoint& size);

	void		Update(const float deltaTime);

public:

	// Place getters and setters in their own public field

	SDL_FRect&	GetRect(void) const							{return (SDL_FRect&)rect;}

	void		SetTarget(const SDL_FPoint& targetPosition) {target = targetPosition;}

private:

	SDL_FRect	rect		= {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_FPoint	target		= {0.0f, 0.0f};

	/**
	* How fast the camera moves towards the target
	* Play around with this value until you like the speed
	* The higher this value is, the faster the camera will move towards the target
	*/
	float		easeFactor	= 2.0f;

};