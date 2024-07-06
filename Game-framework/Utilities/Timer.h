#pragma once

class Timer
{
public:

	 Timer(void) {}
	~Timer(void) {}

	void	Update();

public:

	// Place getters and setters in their own public field

	double	GetDeltaTime(void) const {return deltaTime;}

private:

	double currentTime	= 0.0;
	double previousTime	= 0.0;
	double deltaTime	= 0.0;

};

