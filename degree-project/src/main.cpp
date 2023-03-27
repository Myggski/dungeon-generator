#define SDL_MAIN_HANDLED

#include "Application/Application.h"
#include "LevelGeneration/Cyclic/CyclicInsertionPoint.h"
#include "LevelGeneration/Cyclic/CyclicRule.h"
#include "LevelGeneration/Cyclic/CyclicRuleRepository.h"
#include "LevelGeneration/MazeGenerator/Maze.h"

int main(int argc, char* args[])
{
	Application::Application App {  };
	App.Init();

	/**/


	App.Run();

	return 0;
}
