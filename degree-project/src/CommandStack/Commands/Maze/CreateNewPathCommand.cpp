#include "CreateNewPathCommand.h"

#include "LevelGeneration/MazeGenerator/NavigationalDirections.h"

namespace MazeGenerator
{
	class Maze;
}

namespace Command
{
	CreateNewPathCommand::CreateNewPathCommand(MazeGenerator::MazeStateData& StateData, std::array<DirectionType, 4> RandomDirections)
		: StateData(StateData),
		RandomDirections(RandomDirections)
	{
		ChangedCells.reserve(4);
	}

	CreateNewPathCommand::~CreateNewPathCommand()
	{
		ChangedCells.clear();
	}

	void CreateNewPathCommand::Execute()
	{
		// Set neighbors of the goal to unvisited
		for (DirectionType Direction : RandomDirections)
		{
			const int NeighborX = StateData.GoalCell->GetPosition().x + MazeGenerator::DirectionToGridStepX.at(Direction);
			const int NeighborY = StateData.GoalCell->GetPosition().y + MazeGenerator::DirectionToGridStepY.at(Direction);

			if (!IsOutOfBound(StateData, NeighborX, NeighborY))
			{
				MazeGenerator::MazeCell* NeighborCell = &StateData.MazeGrid[NeighborX][NeighborY];

				if (NeighborCell->IsVisited())
				{
					ChangedCells.emplace_back(NeighborCell);
					NeighborCell->SetVisited(false);
				}
			}
		}

		StateData.CurrentPathwayIndex++;
	}

	void CreateNewPathCommand::Undo()
	{
		for (MazeGenerator::MazeCell* ChangedCell : ChangedCells)
		{
			ChangedCell->SetVisited(true);
		}

		StateData.CurrentPathwayIndex--;
	}
}
