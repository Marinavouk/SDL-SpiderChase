#pragma once

class Application;

// The base class that every state in the application/game inherits from
class State
{
public:

			 State(void)														{}
			 State(Application* mainApplication) : application(mainApplication)	{}
	virtual ~State(void)														{}

	virtual bool OnEnter(void)					{return true;}
	virtual void OnExit(void)					{}
	virtual void Update(const float deltaTime)	{}
	virtual void Render(void)					{}

protected:

	Application* application = nullptr;

};