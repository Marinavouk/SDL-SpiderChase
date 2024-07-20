#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>

class Application;

class Player
{
public:

	 Player() {}
	~Player() {}

	bool Create(Application* mainApplication);
	void Destroy();
	void HandleInput(const float deltaTime);
	void Update(const float deltaTime, const std::vector<SDL_FRect>& levelColliders);
	void Render(void);

	float GetYPosition() { return yPosition; }
	float GetScore() { return highestScore; }

	bool GetIsJumping() { return jumping; }

	void IsDead();

private:

	void SyncColliders();

private:

	Application* application = nullptr;

	// Player texture
	SDL_Texture* texture = nullptr;

	Mix_Chunk* jumpSound = nullptr;

public:

	SDL_FRect collider = { 0.0f, 0.0f, 0.0f, 0.0f };

private:

	float xPosition = 0.0f;
	float yPosition = 0.0f;
	float gravity = 1500.0f;
	float xVelocity = 0.0f;
	float yVelocity = 0.0f;
	float movementSpeed = 350.0f;
	float jumpStrength = 700.0f;
	float score = 0.0f;
	float highestScore = 0.0f;

	int direction = 1; // 0 = facing left, 1 = facing right

	bool jumping = false;

};

