#include "LevelGenerator.h"

#include <array>

#include "Application/Font.h"
#include "CommandStack/CommandStack.h"
#include "CommandStack/Commands/Maze/AddElementsCommand.h"
#include "CommandStack/Commands/Maze/AddSideRoomsCommand.h"
#include "CommandStack/Commands/Maze/CarvePassageCommand.h"
#include "CommandStack/Commands/Maze/BacktrackPassageCommand.h"
#include "CommandStack/Commands/Maze/CreateNewPathCommand.h"
#include "CommandStack/Commands/Maze/DeadEndCommand.h"

namespace LevelGenerator
{
	LevelGenerator::LevelGenerator(Cyclic::CyclicRule MainRule, int Width, int Height)
		: StateData(Width, Height),
		MainRule(std::move(MainRule)) { }

	LevelGenerator& LevelGenerator::operator=(LevelGenerator&& Other) noexcept
	{
		MainRule = Other.MainRule;
		StateData = { Other.StateData.GridWidth, Other.StateData.GridHeight };

		InitializeMaze();

		return *this;
	}

	GeneratorActionType LevelGenerator::Step()
	{
		if (StateData.CurrentAction == GeneratorActionType::FillEmptySlots)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::AddSideRoomsCommand>(
					StateData,
					NavigationalDirections::GetRandomDirections(),
					!Utils::RandomGenerator::GetInstance().GetRandom(0, 3) // 25% chance to add a room
				)
			);
		}

		if (StateData.CurrentAction == GeneratorActionType::AddMainRuleElements)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::AddElementCommand>(
					MainRule,
					StateData
				)
			);
		}

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
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::CreateNewPathCommand>(
					StateData
				)
			);
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
		}

		CalculateStepsLeft();

		return StateData.CurrentAction;
	}

	void LevelGenerator::TryCarvePassage()
	{
		const DirectionType MoveTowardsDirection = CalculateNextDirection();

		if (MoveTowardsDirection == DirectionType::None)
		{
			StateData.CurrentAction = GeneratorActionType::ReachedDeadEnd;

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

	std::vector<DirectionType> GetCellsWithWeight(const std::vector<std::tuple<DirectionType, float>>& WeightedNeighborCells, float Weight)
	{
		std::vector<DirectionType> Directions;
		Directions.reserve(WeightedNeighborCells.size());

		for (const auto& [Direction, DirectionWeight] : WeightedNeighborCells)
		{
			if (DirectionWeight == Weight)
			{
				Directions.emplace_back(Direction);
			}
		}
		
		return Directions;
	}

	std::vector<std::tuple<DirectionType, float>> LevelGenerator::GetWeightedDirections(const std::vector<DirectionType>& AvailableDirections)
	{
		std::vector<std::tuple<DirectionType, float>> WeightedNeighborCells;
		WeightedNeighborCells.reserve(AvailableDirections.size());

		for (const DirectionType Direction : AvailableDirections)
		{
			/*if (!bIsShortArc && AvailableDirections.size() > 1 && PathwayCalculationData.NumberOfStepsTaken > 3)
			{
				if (StateData.PreviousDirections.size() > 1 && StateData.PreviousDirections.back() == StateData.PreviousDirections.front() && StateData.PreviousDirections.front() == Direction)
				{
					continue;
				}
			}*/

			LevelCell* NeighborCell = StateData.GetNeighborCell(StateData.CurrentCell, Direction);

			const int StepsToGoalX = abs(NeighborCell->GetPosition().x - StateData.GoalCell->GetPosition().x);
			const int StepsToGoalY = abs(NeighborCell->GetPosition().y - StateData.GoalCell->GetPosition().y);

			WeightedNeighborCells.emplace_back(Direction, StepsToGoalX + StepsToGoalY);
		}

		// Descending (From furthest to nearest)
		std::ranges::sort(WeightedNeighborCells,
			[](const auto& A, const auto& B)
			{
				const auto& [DirectionA, WeightA] = A;
				const auto& [DirectionB, WeightB] = B;
				return WeightA > WeightB;
			});

		return WeightedNeighborCells;
	}

	DirectionType LevelGenerator::CalculateNextDirection()
	{
		const bool bIsShortArc = MainRule.GetArcType(StateData.CurrentInsertionIndex) == Cyclic::ArcType::Short;
		const std::vector<DirectionType> AvailableDirections = StateData.GetAvailableDirections(StateData.CurrentCell);

		if (AvailableDirections.empty())
		{
			return DirectionType::None;
		}

		std::vector<std::tuple<DirectionType, float>> WeightedNeighborCells = GetWeightedDirections(AvailableDirections);
		const float MaxWeight = std::get<1>(WeightedNeighborCells[0]);
		const float MinWeight = std::get<1>(WeightedNeighborCells[WeightedNeighborCells.size() - 1]);

		const float StepsLeft = static_cast<float>(PathwayCalculationData.MaxSteps + 2 - PathwayCalculationData.NumberOfStepsTaken);
		const float StepsLeftPercentage = StepsLeft / static_cast<float>(PathwayCalculationData.MaxSteps + 2);

		const bool bShortPathFirstStep = bIsShortArc && StepsLeftPercentage == 1.f;
		const bool bLongPathFirstSteps = PathwayCalculationData.NumberOfStepsTaken < (StateData.GridWidth + StateData.GridHeight) / 4;

		const float LongPathThreshold = bIsShortArc ? 0.9f : 0.8f;
		const float ShortPathThreshold = bIsShortArc ? 0.8f : 0.65f;

		if (bShortPathFirstStep)
		{
			std::vector<DirectionType> Directions = GetCellsWithWeight(WeightedNeighborCells, MinWeight);
			std::ranges::shuffle(Directions, Utils::RandomGenerator::GetInstance().GetEngine());

			return Directions[0];
		}

		if (bLongPathFirstSteps)
		{
			std::vector<DirectionType> Directions = GetCellsWithWeight(WeightedNeighborCells, MaxWeight);

			if (!StateData.PreviousDirections.empty() && std::ranges::find(Directions, StateData.PreviousDirections.back()) != Directions.end())
			{
				return StateData.PreviousDirections.back();
			}

			std::ranges::shuffle(Directions, Utils::RandomGenerator::GetInstance().GetEngine());

			return Directions[0];
		}

		if (StepsLeftPercentage > LongPathThreshold) // Take the long way
		{
			std::vector<DirectionType> Directions = GetCellsWithWeight(WeightedNeighborCells, MaxWeight);

			std::ranges::shuffle(Directions, Utils::RandomGenerator::GetInstance().GetEngine());

			return Directions[0];
		}
		else if (StepsLeftPercentage <= ShortPathThreshold) // Take short way
		{
			std::vector<DirectionType> Directions = GetCellsWithWeight(WeightedNeighborCells, MinWeight);

			std::ranges::shuffle(Directions, Utils::RandomGenerator::GetInstance().GetEngine());

			return Directions[0];
		}
		else // Take random available direction
		{
			std::ranges::shuffle(WeightedNeighborCells, Utils::RandomGenerator::GetInstance().GetEngine());
			return std::get<0>(WeightedNeighborCells[0]);
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

	void LevelGenerator::InitializeMaze()
	{
		StateData.LevelGrid.resize(StateData.GridWidth);

		for (int GridX = 0; GridX < StateData.GridWidth; GridX++)
		{
			StateData.LevelGrid[GridX].resize(StateData.GridHeight);

			for (int GridY = 0; GridY < StateData.GridHeight; GridY++)
			{
				StateData.LevelGrid[GridX][GridY] = LevelCell(SDL_Point{ GridX, GridY });
			}
		}

		// Start with the short path
		if (MainRule.HasArcType(Cyclic::ArcType::Short))
		{
			if (MainRule.GetArcType(0) != Cyclic::ArcType::Short)
			{
				MainRule.ReverseInsertionPoints();
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
		StateData.GoalCell = GetRandomGoalCell(OppositeDirection.at(StartPositionDirection));
		PathwayCalculationData = PathwayData();

		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CreateNewPathCommand>(
				StateData
			)
		);

		const Cyclic::ArcType CurrentArcType = MainRule.GetArcType(StateData.CurrentInsertionIndex);
		const auto [Min, Max] = CalculateMinMaxSteps(CurrentArcType);

		PathwayCalculationData.MinSteps = Min;
		PathwayCalculationData.MaxSteps = Max;

		CalculateStepsLeft();
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
			return &StateData.LevelGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, StateData.GridWidth - 1)][0];
		case DirectionType::East:
			return &StateData.LevelGrid[StateData.GridWidth - 1][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, StateData.GridHeight - 1)];
			break;
		case DirectionType::South:
			return &StateData.LevelGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, StateData.GridWidth - 1)][StateData.GridHeight - 1];
		case DirectionType::West:
		default:
			return &StateData.LevelGrid[0][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, StateData.GridHeight - 1)];
		}
	}

	int Wrap(int Value, int MaxValue)
	{
		return (Value % (MaxValue + 1) + (MaxValue + 1)) % (MaxValue + 1);
	}

	LevelCell* LevelGenerator::GetRandomGoalCell(DirectionType Direction)
	{
		LevelCell* PotentialGoalCell = GetRandomEdgeCell(Direction);

		const int PositionX = PotentialGoalCell->GetPosition().x;
		const int PositionY = PotentialGoalCell->GetPosition().y;
		int MovePositionX = 0;
		int MovePositionY = 0;
		
		if (PositionX == StateData.StartCell->GetPosition().x)
		{
			int MoveSteps = Utils::RandomGenerator::GetInstance().GetRandom(1, 2);

			if (Utils::RandomGenerator::GetInstance().GetRandom(0, 1))
			{
				MoveSteps = -MoveSteps;
			}

			MovePositionX += MoveSteps;
		}
		else if (PositionY == StateData.StartCell->GetPosition().y)
		{
			int MoveSteps = Utils::RandomGenerator::GetInstance().GetRandom(1, 2);

			if (Utils::RandomGenerator::GetInstance().GetRandom(0, 1))
			{
				MoveSteps = -MoveSteps;
			}

			MovePositionY += MoveSteps;
		}

		PotentialGoalCell = &StateData.LevelGrid[(PositionX + MovePositionX % StateData.GridWidth + StateData.GridWidth) % StateData.GridWidth][(PositionY + MovePositionY % StateData.GridHeight + StateData.GridHeight) % StateData.GridHeight];

		if (MainRule.HasArcType(Cyclic::ArcType::Short))
		{
			const NavigationalDirections Directions = GetCellDirection(*PotentialGoalCell, *StateData.StartCell);

			if (std::abs(StateData.StartCell->GetPosition().x - PotentialGoalCell->GetPosition().x) > std::abs(StateData.StartCell->GetPosition().y - PotentialGoalCell->GetPosition().y))
			{
				MovePositionX += DirectionToGridStepX.at(Directions.GetHorizontal());
			}
			else if (std::abs(StateData.StartCell->GetPosition().y - PotentialGoalCell->GetPosition().y) > std::abs(StateData.StartCell->GetPosition().x - PotentialGoalCell->GetPosition().x))
			{
				MovePositionY += DirectionToGridStepY.at(Directions.GetVertical());
			}
		}

		return &StateData.LevelGrid[(PositionX + MovePositionX % StateData.GridWidth + StateData.GridWidth) % StateData.GridWidth][(PositionY + MovePositionY % StateData.GridHeight + StateData.GridHeight) % StateData.GridHeight];
	}

	std::vector<std::vector<LevelCell>>& LevelGenerator::GetLevelGrid()
	{
		return StateData.LevelGrid;
	}

	std::tuple<int, int> LevelGenerator::CalculateMinMaxSteps(Cyclic::ArcType ArcType) const
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

		const int StepsToGoalX = abs(StateData.StartCell->GetPosition().x - StateData.GoalCell->GetPosition().x);
		const int StepsToGoalY = abs(StateData.StartCell->GetPosition().y - StateData.GoalCell->GetPosition().y);
		const int TotalSteps = StepsToGoalX + StepsToGoalY;

		return std::make_tuple(
			static_cast<int>(static_cast<float>(TotalSteps) * MinPercentage),
			static_cast<int>(static_cast<float>(TotalSteps) * MaxPercentage)
		);
	}

	int LevelGenerator::GetGridWidth() const
	{
		return StateData.GridWidth;
	}

	int LevelGenerator::GetGridHeight() const
	{
		return StateData.GridHeight;
	}
}
