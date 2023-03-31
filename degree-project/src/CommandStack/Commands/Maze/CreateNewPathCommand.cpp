#include "CreateNewPathCommand.h"

#include "LevelGeneration/LevelGenerator/NavigationalDirections.h"

namespace LevelGenerator
{
	class LevelGenerator;
}

namespace Command
{
	CreateNewPathCommand::CreateNewPathCommand(LevelGenerator::LevelStateData& StateData)
		: StateData(StateData),
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
		if (!StateData.StartCell->IsVisited())
		{
			ChangedCells.emplace_back(StateData.StartCell);
		}

		if (StateData.GoalCell->IsVisited())
		{
			ChangedCells.emplace_back(StateData.StartCell);
		}

		PreviousActionType = StateData.CurrentAction;
		PreviousCurrentCell = StateData.CurrentCell;

		StateData.StartCell->SetVisited(true);
		StateData.CurrentCell = StateData.StartCell;
		StateData.VisitedCellStack.push(StateData.CurrentCell);
		StateData.GoalCell->SetVisited(false);
		StateData.CurrentAction = LevelGenerator::GeneratorActionType::CarvingPath;

		// Set neighbors of the goal to unvisited
		for (DirectionType Direction : LevelGenerator::NavigationalDirections::GetDirections())
		{
			const int NeighborX = StateData.GoalCell->GetPosition().x + LevelGenerator::DirectionToGridStepX.at(Direction);
			const int NeighborY = StateData.GoalCell->GetPosition().y + LevelGenerator::DirectionToGridStepY.at(Direction);

			if (!IsOutOfBound(StateData, NeighborX, NeighborY))
			{
				LevelGenerator::LevelCell* NeighborCell = &StateData.MazeGrid[NeighborX][NeighborY];

				if (NeighborCell->IsVisited())
				{
					ChangedCells.emplace_back(NeighborCell);
					NeighborCell->SetVisited(false);
				}
			}
		}

		while (!StateData.PreviousDirections.empty())
		{
			StateData.PreviousDirections.pop();
		}

		StateData.CurrentInsertionIndex++;
	}

	void CreateNewPathCommand::Undo()
	{
		for (LevelGenerator::LevelCell* ChangedCell : ChangedCells)
		{
			ChangedCell->SetVisited(!ChangedCell->IsVisited());
		}

		if (!StateData.VisitedCellStack.empty())
		{
			StateData.VisitedCellStack.pop();
		}

		if (PreviousCurrentCell)
		{
			StateData.CurrentCell = PreviousCurrentCell;
		}
		
		StateData.PreviousDirections = PreviousDirections;
		StateData.CurrentAction = PreviousActionType;
		StateData.CurrentInsertionIndex--;
	}
}
