#include "CarvePassageCommand.h"

#include "LevelGeneration/MazeGenerator/NavigationalDirections.h"
#include "LevelGeneration/MazeGenerator/MazeCell.h"

namespace MazeGenerator
{
	class Maze;
}

namespace Command
{
	CarvePassageCommand::CarvePassageCommand(MazeGenerator::MazeStateData& StateData, DirectionType MoveTowardsDirection)
		: StateData(StateData),
		CarvedFromCell(nullptr),
		CarvedToCell(nullptr),
		MoveTowardsDirection(MoveTowardsDirection),
		PreviousAction(MazeGenerator::MazeActionType::None) { }

	CarvePassageCommand::~CarvePassageCommand()
	{
		CarvedFromCell = nullptr;
		CarvedToCell = nullptr;
	}
	
	void CarvePassageCommand::Execute()
	{
		const int NeighborX = StateData.CurrentCell->GetPosition().x + MazeGenerator::DirectionToGridStepX.at(MoveTowardsDirection);
		const int NeighborY = StateData.CurrentCell->GetPosition().y + MazeGenerator::DirectionToGridStepY.at(MoveTowardsDirection);

		CarvedFromCell = StateData.CurrentCell;
		CarvedToCell = &StateData.MazeGrid[NeighborX][NeighborY];

		CarvedFromCell->CarveEntrance(MoveTowardsDirection);

		CarvedToCell->SetVisited(true);
		CarvedToCell->CarveEntrance(MazeGenerator::OppositeDirection.at(MoveTowardsDirection));
		StateData.VisitedCellStack.push(CarvedToCell);

  		StateData.CurrentCell = CarvedToCell;
		StateData.PreviousDirection = MoveTowardsDirection;

		if (StateData.CurrentCell == StateData.GoalCell)
		{
			StateData.CurrentAction = MazeGenerator::MazeActionType::BacktrackPassage;
			StateData.GetCurrentPathway().reserve(StateData.VisitedCellStack.size());
		}
	}

	void CarvePassageCommand::Undo()
	{
		CarvedFromCell->CollapseEntrance(MoveTowardsDirection);

		CarvedToCell->SetVisited(false);
		CarvedToCell->CollapseEntrance(MazeGenerator::OppositeDirection.at(MoveTowardsDirection));
		StateData.VisitedCellStack.pop();

		StateData.CurrentCell = CarvedFromCell;

		if (PreviousAction != MazeGenerator::MazeActionType::None)
		{
			StateData.CurrentAction = PreviousAction;
		}
	}
}
