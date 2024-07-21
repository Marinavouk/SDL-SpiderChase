#include "Player.h"
#include <iostream>
#include "Application.h"

bool Player::Create(Application* mainApplication)
{
	application = mainApplication;

	texture = application->GetTextureHandler().CreateTexture("Assets/Textures/triangleTest.png");
	if (!texture)
		return false;

	int playerWidth = 0;
	int playerHeight = 0;
	SDL_QueryTexture(texture, nullptr, nullptr, &playerWidth, &playerHeight);

	collider = { 0.0f, 0.0f, (float)(playerWidth * 0.7f), (float)(playerHeight * 0.7f) };

	xPosition = application->GetWindow().GetWidth() * 0.5f;
	yPosition = application->GetWindow().GetHeight() - collider.h;

	collider.x = xPosition;
	collider.y = yPosition;

	return true;
}

void Player::Destroy()
{
	application->GetTextureHandler().DestroyTexture(texture);
	texture = nullptr;
	//application->GetAudioHandler()->DestroyAudio(jumpSound);
}

void Player::HandleInput(const float deltaTime)
{
	if (application->GetInputHandler().KeyHeld(SDL_SCANCODE_LEFT))
	{
		xVelocity -= movementSpeed * deltaTime;
		direction = 0;
	}
	else if (application->GetInputHandler().KeyHeld(SDL_SCANCODE_RIGHT))
	{
		xVelocity += movementSpeed * deltaTime;
		direction = 1;
	}

	xPosition += xVelocity * deltaTime;

}


void Player::Update(const float deltaTime, const std::vector<SDL_FRect>& levelColliders)
{
	HandleInput(deltaTime);

	const float windowWidth = (float)application->GetWindow().GetWidth();
	const float windowHeight = (float)application->GetWindow().GetHeight();

	xPosition += xVelocity * deltaTime;
	yVelocity += gravity * deltaTime;
	yPosition += yVelocity * deltaTime;

	// Make sure that the player can't leave the window's left- and right edges
	if (xPosition < 0.0f)
		xPosition = 0.0f;
	else if (xPosition > windowWidth - collider.w)
		xPosition = windowWidth - collider.w;

	if (yPosition > (windowHeight - collider.h))
	{
		yPosition = windowHeight - collider.h;
		yVelocity = 0.0f;
		jumping = false;
	}

	SyncColliders();
}

void Player::Render(void)
{
	TextureHandler& textureHandler = application->GetTextureHandler();

	textureHandler.RenderTexture(texture, { xPosition, yPosition });
}

void Player::SyncColliders()
{
	collider.x = xPosition;
	collider.y = yPosition;
}
void Player::IsDead()
{
	
}