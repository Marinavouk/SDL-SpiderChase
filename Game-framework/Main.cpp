#include "Application.h"

// IOS - AppMain
// Android - Main
// Linux - void main
int main(int argc, char* argv[]) //entry point on Windows platform/Operating system
{
	// Every 'new' requires a 'delete' (see below)
	Application* application = new Application;

	if (application->Create())
	{
		application->Run();
		application->Destroy();
	}

	// Avoid memory leak
	delete application;
	application = nullptr;

	return 0;
}