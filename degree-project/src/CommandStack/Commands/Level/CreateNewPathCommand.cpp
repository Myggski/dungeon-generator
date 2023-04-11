#include "CreateNewPathCommand.h"

#include "LevelGeneration/LevelGenerator/RuleLevel/NavigationalDirections.h"

namespace LevelGenerator
{
	class LevelGenerator;
}

namespace Command
{
	CreateNewPathCommand::CreateNewPathCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData)
		: RuleLevelStateData(RuleLevelStateData),
		PreviousCurrentCell(nullptr),
		PreviousActionType(LevelGenerator::GeneratorActionType::None)
	{
		ChangedCells.reserve(6);
	}

	CreateNewPathCommand::~CreateNewPathCommand()
	{
		ChangedCells.clear();
	}

	void CreateNewPathCommand::Execute()
	{
		if (!RuleLevelStateData.StartCell->IsVisited())
		{
			ChangedCells.emplace_back(RuleLevelStateData.StartCell);
		}

		if (RuleLevelStateData.GoalCell->IsVisited())
		{
			ChangedCells.emplace_back(RuleLevelStateData.StartCell);
		}

		PreviousActionType = RuleLevelStateData.CurrentAction;
		PreviousCurrentCell = RuleLevelStateData.CurrentCell;

		RuleLevelStateData.StartCell->SetVisited(true);
		RuleLevelStateData.CurrentCell = RuleLevelStateData.StartCell;
		RuleLevelStateData.VisitedCellStack.push(RuleLevelStateData.CurrentCell);
		RuleLevelStateData.GoalCell->SetVisited(false);
		RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::CarvingPath;

		// Set neighbors of the goal to unvisited
		for (DirectionType Direction : LevelGenerator::NavigationalDirections::GetDirections())
		{
			const int NeighborX = RuleLevelStateData.GoalCell->GetPosition().x + LevelGenerator::DirectionToGridStepX.at(Direction);
			const int NeighborY = RuleLevelStateData.GoalCell->GetPosition().y + LevelGenerator::DirectionToGridStepY.at(Direction);

			if (!RuleLevelStateData.IsOutOfBound(NeighborX, NeighborY))
			{
				LevelGenerator::RuleLevelCell* NeighborCell = &RuleLevelStateData.LevelGrid[NeighborX][NeighborY];

				if (NeighborCell->IsVisited())
				{
					//ChangedCells.emplace_back(NeighborCell);
					//NeighborCell->SetVisited(false);
				}
			}
		}

		while (!RuleLevelStateData.PreviousDirections.empty())
		{
			RuleLevelStateData.PreviousDirections.pop();
		}

		RuleLevelStateData.CurrentInsertionIndex++;
	}

	void CreateNewPathCommand::Undo()
	{
		for (LevelGenerator::RuleLevelCell* ChangedCell : ChangedCells)
		{
			ChangedCell->SetVisited(!ChangedCell->IsVisited());
		}

		if (!RuleLevelStateData.VisitedCellStack.empty())
		{
			RuleLevelStateData.VisitedCellStack.pop();
		}

		if (PreviousCurrentCell)
		{
			RuleLevelStateData.CurrentCell = PreviousCurrentCell;
		}
		
		RuleLevelStateData.PreviousDirections = PreviousDirections;
		RuleLevelStateData.CurrentAction = PreviousActionType;
		RuleLevelStateData.CurrentInsertionIndex--;
	}
}
