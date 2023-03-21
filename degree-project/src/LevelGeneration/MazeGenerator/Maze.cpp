#include "Maze.h"

#include <array>

#include "Application/Font.h"
#include "CommandStack/CommandStack.h"
#include "CommandStack/Commands/CarvePassageCommand.h"

namespace Command
{
	class CarvePassageCommand;
}

namespace MazeGenerator
{
	Maze::Maze(int Width, int Height) : Width(Width),
		Height(Height),
		bGoalHasBeenReached(false),
		CurrentCell(nullptr),
		StartCell(nullptr),
		GoalCell(nullptr)
	{
		Grid.resize(Width);

		for (int GridX = 0; GridX < Width; GridX++)
		{
			Grid[GridX].resize(Height);

			for (int GridY = 0; GridY < Height; GridY++)
			{
				Grid[GridX][GridY] = MazeCell(SDL_Point{ GridX, GridY });
			}
		}

		InitializePath();
	}

	void Maze::CarveCellPassage()
	{
 		if (VisitedStack.empty())
		{
			if (bGoalHasBeenReached)
			{
				bGoalHasBeenReached = false;
				StartCell->SetVisited(true);
				VisitedStack.push(StartCell);
				CalculateStepsLeft();

				GoalCell->SetVisited(false);

				for (DirectionType Direction : GetRandomDirections())
				{
					const int NeighborX = GoalCell->GetPosition().x + DirectionToGridStepX.at(Direction);
					const int NeighborY = GoalCell->GetPosition().y + DirectionToGridStepY.at(Direction);

					if (!IsOutOfBound(NeighborX, NeighborY))
					{
						Grid[NeighborX][NeighborY].SetVisited(false);
					}
				}
			}

			return;
		}

		const bool bNewPathCreated = TryMakePathToNeighbor();

		// Backtrack if there's no new path that has been created
		if (!bNewPathCreated)
		{
			BacktrackVisitedPath();
		} // If a new path has been created, check if goal has been reached
		else if (CurrentCell == GoalCell)
		{
			SetGoalReached();
		}
	
		if (!bGoalHasBeenReached)
		{
			CalculateStepsLeft();
		}
	}

	void Maze::SetCurrentCell(MazeCell* NewCurrentCell)
	{
		CurrentCell = NewCurrentCell;
	}

	void Maze::DrawDebugText()
	{
		const std::string GoalReached = bGoalHasBeenReached ? "True" : "False";
		Application::Font::GetInstance().Draw("Default", "Random Seed: " + std::to_string(Utils::RandomGenerator::GetInstance().GetSeed()), 8, 720 - 32 * 7);
		Application::Font::GetInstance().Draw("Default", "Goal Reached: " + GoalReached, 8, 720 - 32 * 6);
		Application::Font::GetInstance().Draw("Default", "Number Of Steps Taken: " + std::to_string(PathwayCalculationData.NumberOfStepsTaken), 8, 720 - 32 * 5 );
		Application::Font::GetInstance().Draw("Default", "Steps To Goal X: " + std::to_string(PathwayCalculationData.StepsToGoalX), 8, 720 - 32 * 4);
		Application::Font::GetInstance().Draw("Default", "Steps To Goal Y: " + std::to_string(PathwayCalculationData.StepsToGoalY), 8, 720 - 32 * 3);
		Application::Font::GetInstance().Draw("Default", "Min Steps: " + std::to_string(PathwayCalculationData.MinSteps), 8, 720 - 32 * 2);
		Application::Font::GetInstance().Draw("Default", "Max Steps: " + std::to_string(PathwayCalculationData.MaxSteps), 8, 720 - 32);
	}

	std::vector<std::vector<MazeCell>> Maze::GetGrid()
	{
		return Grid;
	}

	MazeCell& Maze::GetCurrentCell() const
	{
		return *CurrentCell;
	}

	MazeCell& Maze::GetStartCell() const
	{
		return *StartCell;
	}

	MazeCell& Maze::GetGoalCell() const
	{
		return *GoalCell;
	}

	MazeCell* Maze::GetCell(int PositionX, int PositionY)
	{
		return &Grid[PositionX][PositionY];
	}

	std::vector<MazeCell*> Maze::GetPathway()
	{
		return Pathway;
	}

	void Maze::BacktrackVisitedPath()
	{
		// If goal has been reached, save pathway on the way back
		if (bGoalHasBeenReached)
		{
			Pathway.emplace_back(VisitedStack.top());
		}

		// Go back to the visited stack
		VisitedStack.pop();

		if (!VisitedStack.empty())
		{
			CurrentCell = VisitedStack.top();
		}
	}

	void Maze::SetGoalReached()
	{
		bGoalHasBeenReached = true;
		Pathway.reserve(VisitedStack.size());
		CalculateStepsLeft();
	}

	bool Maze::TryMakePathToNeighbor()
	{
		if (bGoalHasBeenReached)
		{
			return false;
		}
		
		const DirectionType NextDirection = CalculateNextDirection();

		if (NextDirection == DirectionType::None)
		{
			return false;
		}

		const int NeighborX = CurrentCell->GetPosition().x + MazeGenerator::DirectionToGridStepX.at(NextDirection);
		const int NeighborY = CurrentCell->GetPosition().y + MazeGenerator::DirectionToGridStepY.at(NextDirection);
		MazeCell* MoveTowardsCell = &Grid[NeighborX][NeighborY];

		const std::function SetCurrentCellCallback = [&](MazeCell* NewCurrentCell)
		{
			CurrentCell = NewCurrentCell;
			VisitedStack.push(CurrentCell);
			PreviousDirection = NextDirection;
		};

		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CarvePassageCommand>(
				Command::CarvePassageCommandData {
					CurrentCell,
					MoveTowardsCell,
					NextDirection,
					PreviousDirection,
					SetCurrentCellCallback
				}
			)
		);

		return true;
	}

	DirectionType Maze::CalculateNextDirection() const
	{
		std::vector<DirectionType> AvailableDirections = GetAvailableRandomDirections();

		if (AvailableDirections.empty())
		{
			return DirectionType::None;
		}

		const DirectionType Furthest = GetDirectionFurthestGoal();
		const float StepsLeft = static_cast<float>(PathwayCalculationData.MaxSteps - PathwayCalculationData.NumberOfStepsTaken);
		const float StepsLeftPercentage = StepsLeft / static_cast<float>(PathwayCalculationData.MaxSteps);

		if (StepsLeftPercentage > 0.7f)
		{
			if (PathwayCalculationData.StepsToGoalX < PathwayCalculationData.StepsToGoalY)
			{
				const std::array<DirectionType, 2> Horizontal{ DirectionType::West, DirectionType::East };

				for (DirectionType Direction : Horizontal)
				{
					if (std::ranges::find(AvailableDirections, Direction) != AvailableDirections.end())
					{
						return Direction;
					}
				}
			}
			else if (PathwayCalculationData.StepsToGoalY < PathwayCalculationData.StepsToGoalX)
			{
				const std::array<DirectionType, 2> Vertical{ DirectionType::North, DirectionType::South };

				for (DirectionType Direction : Vertical)
				{
					if (std::ranges::find(AvailableDirections, Direction) != AvailableDirections.end())
					{
						return Direction;
					}
				}
			}
			else if (PreviousDirection != DirectionType::None && std::ranges::find(AvailableDirections, PreviousDirection) != AvailableDirections.end())
			{
				return  PreviousDirection;
			}
		}

		if (StepsLeftPercentage <= 0.6f)
		{
			const DirectionType NearestDirection = GetDirectionFurthestGoal();

			if (std::ranges::find(AvailableDirections, NearestDirection) != AvailableDirections.end())
			{
 				return NearestDirection;
			}
		}

		if (std::ranges::find(AvailableDirections, PreviousDirection) != AvailableDirections.end())
		{
			return PreviousDirection;
		}

  		return AvailableDirections[0];
	}

	DirectionType Maze::GetDirectionFurthestGoal() const
	{
		const NavigationalDirections DirectionToGoal = GetCellDirection(*CurrentCell, *GoalCell);

		if (PathwayCalculationData.StepsToGoalX > PathwayCalculationData.StepsToGoalY)
		{
			return DirectionToGoal.GetHorizontal();
		}
		else if (PathwayCalculationData.StepsToGoalY > PathwayCalculationData.StepsToGoalX)
		{
			return DirectionToGoal.GetVertical();
		}

		return PreviousDirection;
	}

	NavigationalDirections Maze::GetCellDirection(const MazeCell& CurrentCell, const MazeCell& NeighborCell) const
	{
		const int DirectionX = NeighborCell.GetPosition().x - CurrentCell.GetPosition().x;
		const int DirectionY = NeighborCell.GetPosition().y - CurrentCell.GetPosition().y;

		DirectionType DirectionHorizontal = DirectionType::None;
		DirectionType DirectionVertical = DirectionType::None;

		if (DirectionX != 0)
		{
			DirectionHorizontal = DirectionX < 0
				? DirectionType::West
				: DirectionType::East;
		}

		if (DirectionY != 0)
			DirectionVertical = DirectionY < 0
			? DirectionType::North
			: DirectionType::South;

		return { DirectionHorizontal, DirectionVertical };
	}

	void Maze::InitializePath()
	{
		// Which side of the grid should the start position be
		const DirectionType StartPositionDirection = GetRandomDirection();

		StartCell = GetRandomEdgeCell(StartPositionDirection);
		StartCell->SetVisited(true);

		// Set the goal cell to be at the opposite site of the start cell
		GoalCell = GetRandomEdgeCell(OppositeDirection.at(StartPositionDirection));

		CurrentCell = StartCell;
		VisitedStack.push(CurrentCell);

		PathwayCalculationData = PathwayData();
		
		CalculateStepsLeft();

		PathwayCalculationData.MinSteps = static_cast<int>(static_cast<float>(PathwayCalculationData.StepsToGoalX + PathwayCalculationData.StepsToGoalY) * 1.25f);
		PathwayCalculationData.MaxSteps = static_cast<int>(static_cast<float>(PathwayCalculationData.MinSteps) * 1.5f);
	}

	void Maze::CalculateStepsLeft()
	{
		PathwayCalculationData.StepsToGoalX = abs(CurrentCell->GetPosition().x - GoalCell->GetPosition().x);
		PathwayCalculationData.StepsToGoalY = abs(CurrentCell->GetPosition().y - GoalCell->GetPosition().y);
		PathwayCalculationData.NumberOfStepsTaken = std::ranges::max(static_cast<int>(VisitedStack.size() - 1), 0);
		PathwayCalculationData.DirectionsToGoal = GetCellDirection(*CurrentCell, *GoalCell);
	}

	MazeCell* Maze::GetRandomEdgeCell(DirectionType Direction)
	{
		switch (Direction)
		{
		case DirectionType::North:
			return &Grid[Utils::RandomGenerator::GetInstance().GetRandom<int>(1, Width - 2)][0];
		case DirectionType::East:
			return &Grid[Width - 1][Utils::RandomGenerator::GetInstance().GetRandom<int>(1, Height - 2)];
			break;
		case DirectionType::South:
			return &Grid[Utils::RandomGenerator::GetInstance().GetRandom<int>(1, Width - 2)][Height - 1];
		case DirectionType::West:
		default:
			return &Grid[0][Utils::RandomGenerator::GetInstance().GetRandom<int>(1, Height - 2)];
		}
	}

	std::array<DirectionType, 4> Maze::GetRandomDirections() const
	{
		std::array<DirectionType, 4> Directions =
		{
			DirectionType::North,
			DirectionType::East,
			DirectionType::South,
			DirectionType::West
		};

		std::ranges::shuffle(Directions, Utils::RandomGenerator::GetInstance().GetEngine());

		return Directions;
	}

	DirectionType Maze::GetRandomDirection() const
	{
		return GetRandomDirections()[0];
	}

	std::vector<DirectionType> Maze::GetAvailableRandomDirections() const
	{
		std::vector<DirectionType> AvailableDirections;
		AvailableDirections.reserve(4);

		const std::array<DirectionType, 4> Directions = GetRandomDirections();

		for (const auto& CurrentDirection : Directions)
		{
			const int NeighborX = CurrentCell->GetPosition().x + DirectionToGridStepX.at(CurrentDirection);
			const int NeighborY = CurrentCell->GetPosition().y + DirectionToGridStepY.at(CurrentDirection);

			if (IsOutOfBound(NeighborX, NeighborY))
			{
				continue;
			}

			if (Grid[NeighborX][NeighborY].IsVisited())
			{
				continue;
			}

			AvailableDirections.emplace_back(CurrentDirection);
		}

		return AvailableDirections;
	}

	bool Maze::IsOutOfBound(int PositionX, int PositionY) const
	{
		return (PositionX < 0 || PositionX > Width - 1) || (PositionY < 0 || PositionY > Height - 1);
	}
}
