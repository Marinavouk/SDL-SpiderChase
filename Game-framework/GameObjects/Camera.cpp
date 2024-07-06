#include "Camera.h"

void Camera::Create(const SDL_FPoint& startPosition, const SDL_FPoint& size)
{
	rect = {startPosition.x, startPosition.y, size.x, size.y};
}

void Camera::Update(const float deltaTime)
{
	/**
	* Smoothly move the camera towards 'target'
	* The target object will always be in the middle of the camera's view
	*/

	const SDL_FPoint positionDifference = {target.x - (rect.w * 0.5f),								target.y - (rect.h * 0.5f)};
	const SDL_FPoint softPosition		= {rect.x + positionDifference.x * deltaTime * easeFactor,	rect.y + positionDifference.y * deltaTime * easeFactor};

	rect.x = softPosition.x;
	rect.y = softPosition.y;
}