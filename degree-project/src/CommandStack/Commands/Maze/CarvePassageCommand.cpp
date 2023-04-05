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
		RemovedDirection(DirectionType::None),
		PreviousAction(LevelGenerator::GeneratorActionType::None),
		DeadEnds({}) { }

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
			ClearAllDeadEnds();
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

		for (LevelGenerator::LevelCell* DeadEnd : DeadEnds)
		{
			DeadEnd->SetVisited(true);
		}
	}

	void CarvePassageCommand::ClearAllDeadEnds()
	{
		for (int X = 0; X < StateData.GridWidth; X++)
		{
			for (int Y = 0; Y < StateData.GridWidth; Y++)
			{
				LevelGenerator::LevelCell* Cell = &StateData.LevelGrid[X][Y];

				if (Cell->IsVisited() && Cell->GetEntranceFlag() == DirectionType::None)
				{
					DeadEnds.push_back(Cell);
					Cell->SetVisited(false);
				}
			}
		}
	}
}
