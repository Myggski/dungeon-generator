#define SDL_MAIN_HANDLED

#include "application/application.h"

int main(int argc, char* args[])
{
	application::application app {  };

	app.init();
	app.run();

	return 0;
}
