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

	xPosition = application->GetWindow()->GetWidth() * 0.5f;
	yPosition = application->GetWindow()->GetHeight() - collider.h;

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

	
}


void Player::Update(const float deltaTime, const std::vector<SDL_FRect>& levelColliders)
{
	
}

void Player::Render(void)
{
	TextureHandler& textureHandler = application->GetTextureHandler();

	textureHandler.RenderTexture(texture, { 600.0f,400.0f });
}

void Player::SyncColliders()
{
	
}
void Player::IsDead()
{
	
}