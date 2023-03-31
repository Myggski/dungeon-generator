#include "LevelGenerator.h"

#include <array>
#include <iostream>

#include "Application/Font.h"
#include "CommandStack/CommandStack.h"
#include "CommandStack/Commands/Maze/CarvePassageCommand.h"
#include "CommandStack/Commands/Maze/BacktrackPassageCommand.h"
#include "CommandStack/Commands/Maze/CreateNewPathCommand.h"
#include "CommandStack/Commands/Maze/DeadEndCommand.h"

namespace Command
{
	class CarvePassageCommand;
}

namespace LevelGenerator
{
	LevelGenerator::LevelGenerator(Cyclic::CyclicRule& MainRule, int Width, int Height)
		: StateData(Width, Height),
		MainRule(MainRule) { }

	GeneratorActionType LevelGenerator::Step()
	{
		if (StateData.CurrentAction == GeneratorActionType::SavingPath)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::BacktrackPassageCommand>(
					StateData
				)
			);
		}

		if (StateData.CurrentAction == GeneratorActionType::CreatingPath)
		{
			CreateNewPath();
		}

		if (StateData.CurrentAction == GeneratorActionType::CarvingPath)
		{
			TryCarvePassage();
		}

		if (StateData.CurrentAction == GeneratorActionType::ReachedDeadEnd)
		{
			if (StateData.VisitedCellStack.empty())
			{
				return GeneratorActionType::Failed;
			}

			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::DeadEndCommand>(
					StateData
				)
			);

			if (!StateData.VisitedCellStack.empty())
			{
				StateData.CurrentAction = GeneratorActionType::CarvingPath;
			}
		}

		CalculateStepsLeft();

		return StateData.CurrentAction;
	}
	
	const LevelCell* LevelGenerator::GetNeighborCell(LevelCell* From, DirectionType Direction) const
	{
		const int NeighborX = From->GetPosition().x + DirectionToGridStepX.at(Direction);
		const int NeighborY = From->GetPosition().y + DirectionToGridStepY.at(Direction);

		if (IsOutOfBound(StateData, NeighborX, NeighborY))
		{
			return nullptr;
		}

		return &StateData.MazeGrid[NeighborX][NeighborY];
	}


	void LevelGenerator::TryCarvePassage()
	{
		const DirectionType MoveTowardsDirection = CalculateNextDirection();

		if (MoveTowardsDirection == DirectionType::None)
		{
			if (StateData.VisitedCellStack.empty())
			{
				StateData.CurrentAction = GeneratorActionType::Failed;
			}
			else
			{
				StateData.CurrentAction = GeneratorActionType::ReachedDeadEnd;
			}

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

	DirectionType LevelGenerator::CalculateNextDirection() const
	{
		std::vector<DirectionType> AvailableDirections = GetAvailableDirections();
		std::vector<std::pair<DirectionType, float>> WeightedNeighborCells;
		WeightedNeighborCells.reserve(AvailableDirections.size());

		if (AvailableDirections.empty())
		{
			return DirectionType::None;
		}

		for (const DirectionType Direction : AvailableDirections)
		{
			if (AvailableDirections.size() > 1)
			{
				if (StateData.PreviousDirections.size() > 1 && StateData.PreviousDirections.back() == StateData.PreviousDirections.front() && StateData.PreviousDirections.front() == Direction)
				{
					continue;
				}
			}

			const LevelCell* NeighborCell = GetNeighborCell(StateData.CurrentCell, Direction);

			const int StepsToGoalX = abs(NeighborCell->GetPosition().x - StateData.GoalCell->GetPosition().x);
			const int StepsToGoalY = abs(NeighborCell->GetPosition().y - StateData.GoalCell->GetPosition().y);

			WeightedNeighborCells.emplace_back(Direction, StepsToGoalX + StepsToGoalY);
		}

		// Descending (From furthest to nearest)
		std::ranges::sort(WeightedNeighborCells, 
			[](const std::pair<DirectionType, float>& A,
		            const std::pair<DirectionType, float>& B)
					{
						return A.second > B.second;
					});

		const float StepsLeft = static_cast<float>(PathwayCalculationData.MaxSteps - PathwayCalculationData.NumberOfStepsTaken);
		const float StepsLeftPercentage = StepsLeft / static_cast<float>(PathwayCalculationData.MaxSteps);

		if (StepsLeftPercentage > 0.8f) {
			return WeightedNeighborCells[0].first;
		}
		else if (StepsLeftPercentage <= 0.65f)
		{
			return WeightedNeighborCells[WeightedNeighborCells.size() - 1].first;
		}
		else
		{
			std::ranges::shuffle(WeightedNeighborCells, Utils::RandomGenerator::GetInstance().GetEngine());
			return WeightedNeighborCells[0].first;
		}
	}

	NavigationalDirections LevelGenerator::GetCellDirection(const LevelCell& CurrentCell, const LevelCell& NeighborCell) const
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

	void LevelGenerator::InitializeStateData()
	{
		CalculateStepsLeft();
	}

	void LevelGenerator::InitializeMaze()
	{
		StateData.MazeGrid.resize(StateData.GridWidth);

		for (int GridX = 0; GridX < StateData.GridWidth; GridX++)
		{
			StateData.MazeGrid[GridX].resize(StateData.GridHeight);

			for (int GridY = 0; GridY < StateData.GridHeight; GridY++)
			{
				StateData.MazeGrid[GridX][GridY] = LevelCell(SDL_Point{ GridX, GridY });
			}
		}

		// Which side of the grid should the start position be
		DirectionType StartPositionDirection = DirectionType::None;

		if (MainRule.HasArcType(Cyclic::ArcType::Short))
		{
			if (StateData.GridHeight < StateData.GridWidth)
			{
				StartPositionDirection = NavigationalDirections::GetRandomVertical();
			}
			else if (StateData.GridWidth < StateData.GridHeight)
			{
				StartPositionDirection = NavigationalDirections::GetRandomHorizontal();
			}
		}
		else
		{
			if (StateData.GridHeight > StateData.GridWidth)
			{
				StartPositionDirection = NavigationalDirections::GetRandomVertical();
			}
			else if (StateData.GridWidth > StateData.GridHeight)
			{
				StartPositionDirection = NavigationalDirections::GetRandomHorizontal();
			}
		}

		// If the level grid is 
		if (StartPositionDirection == DirectionType::None)
		{
			StartPositionDirection = NavigationalDirections::GetRandomDirection();
		}

		StateData.StartCell = GetRandomEdgeCell(StartPositionDirection);

		// Set the goal cell to be at the opposite site of the start cell
		StateData.GoalCell = GetRandomEdgeCell(OppositeDirection.at(StartPositionDirection));
		PathwayCalculationData = PathwayData();

		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CreateNewPathCommand>(
				StateData
			)
		);

		CalculateStepsLeft();

		const Cyclic::ArcType CurrentArcType = MainRule.GetArcType(StateData.CurrentInsertionIndex);
		const std::pair<int, int> MinMaxSteps = CalculateMinMaxSteps(CurrentArcType);

		PathwayCalculationData.MinSteps = MinMaxSteps.first;
		PathwayCalculationData.MaxSteps = MinMaxSteps.second;
	}

	void LevelGenerator::CreateNewPath()
	{
		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CreateNewPathCommand>(
				StateData
			)
		);

		CalculateStepsLeft();

		const Cyclic::ArcType CurrentArcType = MainRule.GetArcType(StateData.CurrentInsertionIndex);
		const std::pair<int, int> MinMaxSteps = CalculateMinMaxSteps(CurrentArcType);

		PathwayCalculationData.MinSteps = MinMaxSteps.first;
		PathwayCalculationData.MaxSteps = MinMaxSteps.second;
	}

	void LevelGenerator::CalculateStepsLeft()
	{
		PathwayCalculationData.StepsToGoalX = abs(StateData.CurrentCell->GetPosition().x - StateData.GoalCell->GetPosition().x);
		PathwayCalculationData.StepsToGoalY = abs(StateData.CurrentCell->GetPosition().y - StateData.GoalCell->GetPosition().y);
		PathwayCalculationData.NumberOfStepsTaken = std::ranges::max(static_cast<int>(StateData.VisitedCellStack.size() - 1), 0);
		PathwayCalculationData.DirectionsToGoal = GetCellDirection(*StateData.CurrentCell, *StateData.GoalCell);
	}

	LevelCell* LevelGenerator::GetRandomEdgeCell(DirectionType Direction)
	{
		switch (Direction)
		{
		case DirectionType::North:
			return &StateData.MazeGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridWidth - 1)][0];
		case DirectionType::East:
			return &StateData.MazeGrid[StateData.GridWidth - 1][Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridHeight - 1)];
			break;
		case DirectionType::South:
			return &StateData.MazeGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridWidth - 1)][StateData.GridHeight - 1];
		case DirectionType::West:
		default:
			return &StateData.MazeGrid[0][Utils::RandomGenerator::GetInstance().GetRandom<int>(1, StateData.GridHeight - 1)];
		}
	}

	std::vector<DirectionType> LevelGenerator::GetAvailableDirections() const
	{
		std::vector<DirectionType> AvailableDirections;
		AvailableDirections.reserve(4);

		const std::array<DirectionType, 4> Directions = NavigationalDirections::GetDirections();

		for (const auto& CurrentDirection : Directions)
		{
			const LevelCell* Neighbor = GetNeighborCell(StateData.CurrentCell, CurrentDirection);

			if (Neighbor == nullptr || Neighbor->IsVisited())
			{
				continue;
			}

			AvailableDirections.emplace_back(CurrentDirection);
		}

		return AvailableDirections;
	}

	std::vector<DirectionType> LevelGenerator::GetAvailableRandomDirections() const
	{
		std::vector<DirectionType> AvailableDirections = GetAvailableDirections();

		if (AvailableDirections.size() > 1)
		{
			std::ranges::shuffle(AvailableDirections, Utils::RandomGenerator::GetInstance().GetEngine());
		}

		return AvailableDirections;
	}

	bool LevelGenerator::IsOutOfBound(const LevelStateData& StateData, int PositionX, int PositionY)
	{
		return (PositionX < 0 || PositionX > StateData.GridWidth - 1) || (PositionY < 0 || PositionY > StateData.GridHeight - 1);
	}

	std::pair<int, int> LevelGenerator::CalculateMinMaxSteps(Cyclic::ArcType ArcType) const
	{
		float MinPercentage;
		float MaxPercentage;

		if (ArcType == Cyclic::ArcType::Short)
		{
			MinPercentage = 1.25f;
			MaxPercentage = 1.4f;
		}
		else
		{
			MinPercentage = 1.45f;
			MaxPercentage = 1.65f;
		}


		return std::make_pair(
			static_cast<int>(static_cast<float>(PathwayCalculationData.StepsToGoalX + PathwayCalculationData.StepsToGoalY) * MinPercentage),
			static_cast<int>(static_cast<float>(PathwayCalculationData.MinSteps) * MaxPercentage)
		);
	}
}
