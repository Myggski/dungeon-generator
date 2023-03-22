#include "CarvePassageCommand.h"

#include <functional>
#include <utility>

#include "LevelGeneration/MazeGenerator/NavigationalDirections.h"
#include "LevelGeneration/MazeGenerator/MazeCell.h"

namespace MazeGenerator
{
	class Maze;
}

namespace Command
{
	CarvePassageCommand::CarvePassageCommand(CarvePassageCommandData Data)
		: Data(std::move(Data)) { }

	CarvePassageCommand::~CarvePassageCommand()
	{
		Data.CarveFromCell = nullptr;
		Data.CarveToCell = nullptr;
	}
	
	void CarvePassageCommand::Execute()
	{
		Data.CarveFromCell->CarveEntrance(Data.MoveTowardsDirection);

		Data.CarveToCell->CarveEntrance(MazeGenerator::OppositeDirection.at(Data.MoveTowardsDirection));
		Data.CarveToCell->SetVisited(true);
		Data.VisitedStack.push(Data.CarveToCell);

		Data.SetCurrentCellCallback(Data.CarveToCell);
	}

	void CarvePassageCommand::Undo()
	{
		Data.CarveFromCell->CollapseEntrance(Data.MoveTowardsDirection);

		Data.CarveToCell->SetVisited(false);
		Data.CarveToCell->CollapseEntrance(MazeGenerator::OppositeDirection.at(Data.MoveTowardsDirection));
		Data.VisitedStack.pop();

		Data.SetCurrentCellCallback(Data.CarveFromCell);
	}
}
