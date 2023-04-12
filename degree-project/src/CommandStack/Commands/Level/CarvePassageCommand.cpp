#include "CarvePassageCommand.h"

#include "LevelGeneration/LevelGenerator/RuleLevel/NavigationalDirections.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelCell.h"

namespace LevelGenerator
{
	class RuleLevelGenerator;
}

namespace Command
{
	CarvePassageCommand::CarvePassageCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData, DirectionType MoveTowardsDirection)
		: RuleLevelStateData(RuleLevelStateData),
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
		const int NeighborX = RuleLevelStateData.CurrentCell->GetPosition().x + LevelGenerator::DirectionToGridStepX.at(MoveTowardsDirection);
		const int NeighborY = RuleLevelStateData.CurrentCell->GetPosition().y + LevelGenerator::DirectionToGridStepY.at(MoveTowardsDirection);

		CarvedFromCell = RuleLevelStateData.CurrentCell;
		CarvedToCell = &RuleLevelStateData.LevelGrid[NeighborX][NeighborY];

		CarvedFromCell->CarveEntrance(CarvedToCell->GetSpatialHash());

		CarvedToCell->SetVisited(true);
		CarvedToCell->CarveEntrance(CarvedFromCell->GetSpatialHash());
		RuleLevelStateData.VisitedCellStack.push(CarvedToCell);

		RuleLevelStateData.CurrentCell = CarvedToCell;

		if (RuleLevelStateData.PreviousDirections.size() > 1)
		{
			RemovedDirection = RuleLevelStateData.PreviousDirections.front();
		}
		
		RuleLevelStateData.AddPreviousDirection(MoveTowardsDirection);

		if (RuleLevelStateData.CurrentCell == RuleLevelStateData.GoalCell)
		{
			ClearAllDeadEnds();
			RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::SavingPath;
			RuleLevelStateData.GetCurrentPathway().reserve(RuleLevelStateData.VisitedCellStack.size());
		}
	}

	void CarvePassageCommand::Undo()
	{
		CarvedFromCell->CollapseEntrance(CarvedToCell->GetSpatialHash());

		CarvedToCell->SetVisited(false);
		CarvedToCell->CollapseEntrance(CarvedFromCell->GetSpatialHash());
		RuleLevelStateData.VisitedCellStack.pop();

		if (!RuleLevelStateData.PreviousDirections.empty())
		{
			RuleLevelStateData.PreviousDirections.back() = RuleLevelStateData.PreviousDirections.front();
			RuleLevelStateData.PreviousDirections.front() = RemovedDirection;
		}
		
		RuleLevelStateData.CurrentCell = CarvedFromCell;

		if (PreviousAction != LevelGenerator::GeneratorActionType::None)
		{
			RuleLevelStateData.CurrentAction = PreviousAction;
		}

		for (LevelGenerator::RuleLevelCell* DeadEnd : DeadEnds)
		{
			DeadEnd->SetVisited(true);
		}
	}

	void CarvePassageCommand::ClearAllDeadEnds()
	{
		for (int X = 0; X < RuleLevelStateData.GridWidth; X++)
		{
			for (int Y = 0; Y < RuleLevelStateData.GridWidth; Y++)
			{
				LevelGenerator::RuleLevelCell* Cell = &RuleLevelStateData.LevelGrid[X][Y];

				if (Cell->IsVisited() && Cell->GetEntranceFlag() == DirectionType::None)
				{
					DeadEnds.push_back(Cell);
					Cell->SetVisited(false);
				}
			}
		}
	}
}
