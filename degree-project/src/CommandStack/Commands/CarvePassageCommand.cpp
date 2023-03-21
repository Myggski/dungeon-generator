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
		Data.SetCurrentCellCallback = std::nullopt;
	}
	
	void CarvePassageCommand::Execute()
	{
		Data.CarveFromCell->CarveEntrance(Data.MoveTowardsDirection);

		Data.CarveToCell->CarveEntrance(MazeGenerator::OppositeDirection.at(Data.MoveTowardsDirection));
		Data.CarveToCell->SetVisited(true);

		if (Data.SetCurrentCellCallback.has_value())
		{
			Data.SetCurrentCellCallback.value()(Data.CarveToCell);
		}
	}

	void CarvePassageCommand::Undo()
	{
		Data.CarveFromCell->CollapseEntrance(Data.MoveTowardsDirection);

		Data.CarveToCell->SetVisited(false);
		Data.CarveToCell->CollapseEntrance(MazeGenerator::OppositeDirection.at(Data.MoveTowardsDirection));


		if (Data.SetCurrentCellCallback.has_value())
		{
			Data.SetCurrentCellCallback.value()(Data.CarveFromCell);
		}
	}
}
