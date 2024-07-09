#include "Application.h"

int main(int argc, char* argv[])
{
	Application* application = new Application;

	if (application->Create())
	{
		application->Run();
		application->Destroy();
	}

	delete application;
	application = nullptr;

	return 0;
}