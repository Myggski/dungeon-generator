#define SDL_MAIN_HANDLED

#include "Application/Application.h"

int main(int argc, char* args[])
{
	Application::Application App {  };
	App.Init();

	App.Run();

	return 0;
}
