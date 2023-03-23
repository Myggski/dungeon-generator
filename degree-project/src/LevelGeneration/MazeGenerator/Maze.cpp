#include "Maze.h"

#include <array>

#include "imgui.h"
#include "Application/Configuration.h"
#include "Application/Font.h"
#include "CommandStack/CommandStack.h"
#include "CommandStack/Commands/Maze/CarvePassageCommand.h"
#include "CommandStack/Commands/Maze/BacktrackPassageCommand.h"
#include "CommandStack/Commands/Maze/CreateNewPathCommand.h"

namespace Command
{
	class CarvePassageCommand;
}

namespace MazeGenerator
{
	Maze::Maze(int Width, int Height) : StateData(Width, Height)
	{
		StateData.MazeGrid.resize(Width);

		for (int GridX = 0; GridX < Width; GridX++)
		{
			StateData.MazeGrid[GridX].resize(Height);

			for (int GridY = 0; GridY < Height; GridY++)
			{
				StateData.MazeGrid[GridX][GridY] = MazeCell(SDL_Point{ GridX, GridY });
			}
		}

		InitializeMaze();
	}

	MazeActionType Maze::Step()
	{
		if (StateData.CurrentAction == MazeActionType::BacktrackPassage)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::BacktrackPassageCommand>(
					StateData
				)
			);
		}

		if (StateData.CurrentAction == MazeActionType::CreateNewPath)
		{
			CreateNewPath();
		}

		if (StateData.CurrentAction == MazeActionType::CarvePassage)
		{
			TryCarvePassage();
		}
	
		CalculateStepsLeft();

		return StateData.CurrentAction;
	}

	void Maze::DrawDebugText()
	{
		ImGui::SetNextWindowSize({ 208, 172 });
		ImGui::SetNextWindowPos({ 16, static_cast<float>(Configuration::WindowHeight - 172 - 16) });

		ImGui::Begin("Debug");

		ImGui::Text("Seed: %d", Utils::RandomGenerator::GetInstance().GetSeed());
		ImGui::Text("Done: %s", StateData.CurrentAction == MazeActionType::Done ? "True" : "False");
		ImGui::Text("Number Of Steps Taken: %d", PathwayCalculationData.NumberOfStepsTaken);
		ImGui::Text("Steps To Goal X: %d", PathwayCalculationData.StepsToGoalX);
		ImGui::Text("Steps To Goal Y: %d", PathwayCalculationData.StepsToGoalY);
		ImGui::Text("Min Steps: %d", PathwayCalculationData.MinSteps);
		ImGui::Text("Max Steps: %d", PathwayCalculationData.MaxSteps);

		ImGui::End();
	}

	std::vector<std::vector<MazeCell>> Maze::GetGrid()
	{
		return StateData.MazeGrid;
	}

	MazeCell& Maze::GetCurrentCell() const
	{
		return *StateData.CurrentCell;
	}

	MazeCell& Maze::GetStartCell() const
	{
		return *StateData.StartCell;
	}

	MazeCell& Maze::GetGoalCell() const
	{
		return *StateData.GoalCell;
	}

	MazeCell* Maze::GetCell(int PositionX, int PositionY)
	{
		return &StateData.MazeGrid[PositionX][PositionY];
	}

	std::vector<MazeCell*> Maze::GetPathway()
	{
		return StateData.GetCurrentPathway();
	}

	const MazeCell* Maze::GetNeighborCell(MazeCell* From, DirectionType Direction) const
	{
		const int NeighborX = From->GetPosition().x + DirectionToGridStepX.at(Direction);
		const int NeighborY = From->GetPosition().y + DirectionToGridStepY.at(Direction);

		if (IsOutOfBound(StateData, NeighborX, NeighborY))
		{
			return nullptr;
		}

		return &StateData.MazeGrid[NeighborX][NeighborY];
	}


	void Maze::TryCarvePassage()
	{
		const DirectionType MoveTowardsDirection = CalculateNextDirection();

		if (MoveTowardsDirection == DirectionType::None)
		{
			StateData.CurrentAction = MazeActionType::CarvePassageFailed;
			return;
		}
		
		// Move towards direction
		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CarvePassageCommand>(
				StateData,
				MoveTowardsDirection
			)
		);
	}

	DirectionType Maze::CalculateNextDirection() const
	{
		std::vector<DirectionType> AvailableDirections = GetAvailableRandomDirections();

 		if (StateData.CurrentAction == MazeActionType::CarvePassageFailed)
		{
			AvailableDirections.erase(std::ranges::remove(AvailableDirections, StateData.PreviousDirection).begin(), AvailableDirections.end());
		}

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
			else if (StateData.PreviousDirection != DirectionType::None && std::ranges::find(AvailableDirections, StateData.PreviousDirection) != AvailableDirections.end())
			{
				return  StateData.PreviousDirection;
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

		if (std::ranges::find(AvailableDirections, StateData.PreviousDirection) != AvailableDirections.end())
		{
			return StateData.PreviousDirection;
		}

  		return AvailableDirections[0];
	}

	DirectionType Maze::GetDirectionFurthestGoal() const
	{
		const NavigationalDirections DirectionToGoal = GetCellDirection(*StateData.CurrentCell, *StateData.GoalCell);

		if (PathwayCalculationData.StepsToGoalX > PathwayCalculationData.StepsToGoalY)
		{
			return DirectionToGoal.GetHorizontal();
		}
		else if (PathwayCalculationData.StepsToGoalY > PathwayCalculationData.StepsToGoalX)
		{
			return DirectionToGoal.GetVertical();
		}

		return StateData.PreviousDirection;
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

	void Maze::InitializeStateData()
	{
		StateData.StartCell->SetVisited(true);
		StateData.CurrentCell = StateData.StartCell;
		StateData.VisitedCellStack.push(StateData.CurrentCell);
		StateData.GoalCell->SetVisited(false);
		StateData.CurrentAction = MazeActionType::CarvePassage;

		CalculateStepsLeft();
	}

	void Maze::InitializeMaze()
	{
		// Which side of the grid should the start position be
		const DirectionType StartPositionDirection = GetRandomDirection();

		StateData.StartCell = GetRandomEdgeCell(StartPositionDirection);
		// Set the goal cell to be at the opposite site of the start cell
		StateData.GoalCell = GetRandomEdgeCell(OppositeDirection.at(StartPositionDirection));
		PathwayCalculationData = PathwayData();

		InitializeStateData();

		PathwayCalculationData.MinSteps = static_cast<int>(static_cast<float>(PathwayCalculationData.StepsToGoalX + PathwayCalculationData.StepsToGoalY) * 1.25f);
		PathwayCalculationData.MaxSteps = static_cast<int>(static_cast<float>(PathwayCalculationData.MinSteps) * 1.75f);
	}

	void Maze::CreateNewPath()
	{
		InitializeStateData();

		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CreateNewPathCommand>(
				StateData,
				GetRandomDirections()
			)
		);
	}


	void Maze::CalculateStepsLeft()
	{
		PathwayCalculationData.StepsToGoalX = abs(StateData.CurrentCell->GetPosition().x - StateData.GoalCell->GetPosition().x);
		PathwayCalculationData.StepsToGoalY = abs(StateData.CurrentCell->GetPosition().y - StateData.GoalCell->GetPosition().y);
		PathwayCalculationData.NumberOfStepsTaken = std::ranges::max(static_cast<int>(StateData.VisitedCellStack.size() - 1), 0);
		PathwayCalculationData.DirectionsToGoal = GetCellDirection(*StateData.CurrentCell, *StateData.GoalCell);
	}

	MazeCell* Maze::GetRandomEdgeCell(DirectionType Direction)
	{
		switch (Direction)
		{
		case DirectionType::North:
			return &StateData.MazeGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridWidth - 2)][0];
		case DirectionType::East:
			return &StateData.MazeGrid[StateData.GridWidth - 1][Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridHeight - 2)];
			break;
		case DirectionType::South:
			return &StateData.MazeGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridWidth - 2)][StateData.GridHeight - 1];
		case DirectionType::West:
		default:
			return &StateData.MazeGrid[0][Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridHeight - 2)];
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
			const MazeCell* Neighbor = GetNeighborCell(StateData.CurrentCell, CurrentDirection);

			if (Neighbor == nullptr || Neighbor->IsVisited())
			{
				continue;
			}

			AvailableDirections.emplace_back(CurrentDirection);
		}

		return AvailableDirections;
	}

	bool Maze::IsOutOfBound(const MazeStateData& StateData, int PositionX, int PositionY)
	{
		return (PositionX < 0 || PositionX > StateData.GridWidth - 1) || (PositionY < 0 || PositionY > StateData.GridHeight - 1);
	}
}
