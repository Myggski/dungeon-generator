#include "CarvePassageCommand.h"

#include "LevelGeneration/LevelGenerator/NavigationalDirections.h"
#include "LevelGeneration/LevelGenerator/LevelCell.h"

namespace LevelGenerator
{
	class LevelGenerator;
}

namespace Command
{
	CarvePassageCommand::CarvePassageCommand(LevelGenerator::LevelStateData& StateData, DirectionType MoveTowardsDirection)
		: StateData(StateData),
		CarvedFromCell(nullptr),
		CarvedToCell(nullptr),
		MoveTowardsDirection(MoveTowardsDirection),
		PreviousAction(LevelGenerator::GeneratorActionType::None) { }

	CarvePassageCommand::~CarvePassageCommand()
	{
		CarvedFromCell = nullptr;
		CarvedToCell = nullptr;
	}
	
	void CarvePassageCommand::Execute()
	{
		const int NeighborX = StateData.CurrentCell->GetPosition().x + LevelGenerator::DirectionToGridStepX.at(MoveTowardsDirection);
		const int NeighborY = StateData.CurrentCell->GetPosition().y + LevelGenerator::DirectionToGridStepY.at(MoveTowardsDirection);

		CarvedFromCell = StateData.CurrentCell;
		CarvedToCell = &StateData.LevelGrid[NeighborX][NeighborY];

		CarvedFromCell->CarveEntrance(CarvedToCell->GetSpatialHash());

		CarvedToCell->SetVisited(true);
		CarvedToCell->CarveEntrance(CarvedFromCell->GetSpatialHash());
		StateData.VisitedCellStack.push(CarvedToCell);

  		StateData.CurrentCell = CarvedToCell;

		if (StateData.PreviousDirections.size() > 1)
		{
			RemovedDirection = StateData.PreviousDirections.front();
		}
		
		StateData.AddPreviousDirection(MoveTowardsDirection);

		if (StateData.CurrentCell == StateData.GoalCell)
		{
			StateData.CurrentAction = LevelGenerator::GeneratorActionType::SavingPath;
			StateData.GetCurrentPathway().reserve(StateData.VisitedCellStack.size());
		}
	}

	void CarvePassageCommand::Undo()
	{
		CarvedFromCell->CollapseEntrance(CarvedToCell->GetSpatialHash());

		CarvedToCell->SetVisited(false);
		CarvedToCell->CollapseEntrance(CarvedFromCell->GetSpatialHash());
		StateData.VisitedCellStack.pop();

		if (!StateData.PreviousDirections.empty())
		{
			StateData.PreviousDirections.back() = StateData.PreviousDirections.front();
			StateData.PreviousDirections.front() = RemovedDirection;
		}
		
		StateData.CurrentCell = CarvedFromCell;

		if (PreviousAction != LevelGenerator::GeneratorActionType::None)
		{
			StateData.CurrentAction = PreviousAction;
		}
	}
}
