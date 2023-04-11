#include "LevelGenerator.h"

#include <array>

#include "Application/Font.h"
#include "CommandStack/CommandStack.h"
#include "CommandStack/Commands/Level/AddElementsCommand.h"
#include "CommandStack/Commands/Level/AddSideRoomsCommand.h"
#include "CommandStack/Commands/Level/CarvePassageCommand.h"
#include "CommandStack/Commands/Level/BacktrackPassageCommand.h"
#include "CommandStack/Commands/Level/CreateNewPathCommand.h"
#include "CommandStack/Commands/Level/DeadEndCommand.h"

namespace LevelGenerator
{
	LevelGenerator::LevelGenerator(::LevelGenerator::RuleLevelStateData& RuleLevelStateData, Cyclic::CyclicRule MainRule)
		: RuleLevelStateData(RuleLevelStateData),
		MainRule(std::move(MainRule)) { }

	LevelGenerator& LevelGenerator::operator=(LevelGenerator&& Other) noexcept
	{
		RuleLevelStateData = Other.RuleLevelStateData;
		MainRule = Other.MainRule;
		RuleLevelStateData = { Other.RuleLevelStateData.GridWidth, Other.RuleLevelStateData.GridHeight };

		InitializeMaze();

		return *this;
	}

	GeneratorActionType LevelGenerator::Step()
	{
		if (RuleLevelStateData.CurrentAction == GeneratorActionType::FillEmptySlots)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::AddSideRoomsCommand>(
					RuleLevelStateData,
					NavigationalDirections::GetRandomDirections(),
					!Utils::RandomGenerator::GetInstance().GetRandom(0, 3) // 25% chance to add additional sideroom
				)
			);
		}

		if (RuleLevelStateData.CurrentAction == GeneratorActionType::AddMainRuleElements)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::AddElementCommand>(
					MainRule,
					RuleLevelStateData
				)
			);
		}

		if (RuleLevelStateData.CurrentAction == GeneratorActionType::SavingPath)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::BacktrackPassageCommand>(
					RuleLevelStateData
				)
			);
		}

		if (RuleLevelStateData.CurrentAction == GeneratorActionType::CreatingPath)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::CreateNewPathCommand>(
					RuleLevelStateData
				)
			);
		}

		if (RuleLevelStateData.CurrentAction == GeneratorActionType::CarvingPath)
		{
			TryCarvePassage();
		}

		if (RuleLevelStateData.CurrentAction == GeneratorActionType::ReachedDeadEnd)
		{
			if (RuleLevelStateData.VisitedCellStack.empty())
			{
				return GeneratorActionType::Failed;
			}

			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::DeadEndCommand>(
					RuleLevelStateData
				)
			);
		}

		CalculateStepsLeft();

		return RuleLevelStateData.CurrentAction;
	}

	void LevelGenerator::TryCarvePassage()
	{
		const DirectionType MoveTowardsDirection = CalculateNextDirection();

		if (MoveTowardsDirection == DirectionType::None)
		{
			RuleLevelStateData.CurrentAction = GeneratorActionType::ReachedDeadEnd;

			return;
		}
		
		// Move towards direction
		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CarvePassageCommand>(
				RuleLevelStateData,
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
			RuleLevelCell* NeighborCell = RuleLevelStateData.GetNeighborCell(RuleLevelStateData.CurrentCell, Direction);

			const int StepsToGoalX = abs(NeighborCell->GetPosition().x - RuleLevelStateData.GoalCell->GetPosition().x);
			const int StepsToGoalY = abs(NeighborCell->GetPosition().y - RuleLevelStateData.GoalCell->GetPosition().y);

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
		const bool bIsShortArc = MainRule.GetArcType(RuleLevelStateData.CurrentInsertionIndex) == Cyclic::ArcType::Short;
		const std::vector<DirectionType> AvailableDirections = RuleLevelStateData.GetAvailableDirections(RuleLevelStateData.CurrentCell);

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
		const bool bLongPathFirstSteps = PathwayCalculationData.NumberOfStepsTaken < (RuleLevelStateData.GridWidth + RuleLevelStateData.GridHeight) / 4;

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

			if (!RuleLevelStateData.PreviousDirections.empty() && std::ranges::find(Directions, RuleLevelStateData.PreviousDirections.back()) != Directions.end())
			{
				return RuleLevelStateData.PreviousDirections.back();
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

	NavigationalDirections LevelGenerator::GetCellDirection(const RuleLevelCell& CurrentCell, const RuleLevelCell& NeighborCell) const
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
		RuleLevelStateData.LevelGrid.resize(RuleLevelStateData.GridWidth);

		for (int GridX = 0; GridX < RuleLevelStateData.GridWidth; GridX++)
		{
			RuleLevelStateData.LevelGrid[GridX].resize(RuleLevelStateData.GridHeight);

			for (int GridY = 0; GridY < RuleLevelStateData.GridHeight; GridY++)
			{
				RuleLevelStateData.LevelGrid[GridX][GridY] = RuleLevelCell(SDL_Point{ GridX, GridY });
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
			if (RuleLevelStateData.GridHeight < RuleLevelStateData.GridWidth)
			{
				StartPositionDirection = NavigationalDirections::GetRandomVertical();
			}
			else if (RuleLevelStateData.GridWidth < RuleLevelStateData.GridHeight)
			{
				StartPositionDirection = NavigationalDirections::GetRandomHorizontal();
			}
		}
		else
		{
			if (RuleLevelStateData.GridHeight > RuleLevelStateData.GridWidth)
			{
				StartPositionDirection = NavigationalDirections::GetRandomVertical();
			}
			else if (RuleLevelStateData.GridWidth > RuleLevelStateData.GridHeight)
			{
				StartPositionDirection = NavigationalDirections::GetRandomHorizontal();
			}
		}

		// If the level grid is 
		if (StartPositionDirection == DirectionType::None)
		{
			StartPositionDirection = NavigationalDirections::GetRandomDirection();
		}

		RuleLevelStateData.StartCell = GetRandomEdgeCell(StartPositionDirection);

		// Set the goal cell to be at the opposite site of the start cell
		RuleLevelStateData.GoalCell = GetRandomGoalCell(OppositeDirection.at(StartPositionDirection));
		PathwayCalculationData = PathwayData();

		Command::CommandStack::GetInstance().ExecuteCommand(
			std::make_unique<Command::CreateNewPathCommand>(
				RuleLevelStateData
			)
		);

		const Cyclic::ArcType CurrentArcType = MainRule.GetArcType(RuleLevelStateData.CurrentInsertionIndex);
		const auto [Min, Max] = CalculateMinMaxSteps(CurrentArcType);

		PathwayCalculationData.MinSteps = Min;
		PathwayCalculationData.MaxSteps = Max;

		CalculateStepsLeft();
	}

	void LevelGenerator::CalculateStepsLeft()
	{
		PathwayCalculationData.StepsToGoalX = abs(RuleLevelStateData.CurrentCell->GetPosition().x - RuleLevelStateData.GoalCell->GetPosition().x);
		PathwayCalculationData.StepsToGoalY = abs(RuleLevelStateData.CurrentCell->GetPosition().y - RuleLevelStateData.GoalCell->GetPosition().y);
		PathwayCalculationData.NumberOfStepsTaken = std::ranges::max(static_cast<int>(RuleLevelStateData.VisitedCellStack.size() - 1), 0);
		PathwayCalculationData.DirectionsToGoal = GetCellDirection(*RuleLevelStateData.CurrentCell, *RuleLevelStateData.GoalCell);
	}

	RuleLevelCell* LevelGenerator::GetRandomEdgeCell(DirectionType Direction)
	{
		switch (Direction)
		{
		case DirectionType::North:
			return &RuleLevelStateData.LevelGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, RuleLevelStateData.GridWidth - 1)][0];
		case DirectionType::East:
			return &RuleLevelStateData.LevelGrid[RuleLevelStateData.GridWidth - 1][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, RuleLevelStateData.GridHeight - 1)];
			break;
		case DirectionType::South:
			return &RuleLevelStateData.LevelGrid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, RuleLevelStateData.GridWidth - 1)][RuleLevelStateData.GridHeight - 1];
		case DirectionType::West:
		default:
			return &RuleLevelStateData.LevelGrid[0][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, RuleLevelStateData.GridHeight - 1)];
		}
	}

	int Wrap(int Value, int MaxValue)
	{
		return (Value % (MaxValue + 1) + (MaxValue + 1)) % (MaxValue + 1);
	}

	RuleLevelCell* LevelGenerator::GetRandomGoalCell(DirectionType Direction)
	{
		RuleLevelCell* PotentialGoalCell = GetRandomEdgeCell(Direction);

		const int PositionX = PotentialGoalCell->GetPosition().x;
		const int PositionY = PotentialGoalCell->GetPosition().y;
		int MovePositionX = 0;
		int MovePositionY = 0;
		
		if (PositionX == RuleLevelStateData.StartCell->GetPosition().x)
		{
			int MoveSteps = Utils::RandomGenerator::GetInstance().GetRandom(1, 2);

			if (Utils::RandomGenerator::GetInstance().GetRandom(0, 1))
			{
				MoveSteps = -MoveSteps;
			}

			MovePositionX += MoveSteps;
		}
		else if (PositionY == RuleLevelStateData.StartCell->GetPosition().y)
		{
			int MoveSteps = Utils::RandomGenerator::GetInstance().GetRandom(1, 2);

			if (Utils::RandomGenerator::GetInstance().GetRandom(0, 1))
			{
				MoveSteps = -MoveSteps;
			}

			MovePositionY += MoveSteps;
		}

		PotentialGoalCell = &RuleLevelStateData.LevelGrid[(PositionX + MovePositionX % RuleLevelStateData.GridWidth + RuleLevelStateData.GridWidth) % RuleLevelStateData.GridWidth][(PositionY + MovePositionY % RuleLevelStateData.GridHeight + RuleLevelStateData.GridHeight) % RuleLevelStateData.GridHeight];

		if (MainRule.HasArcType(Cyclic::ArcType::Short))
		{
			const NavigationalDirections Directions = GetCellDirection(*PotentialGoalCell, *RuleLevelStateData.StartCell);

			if (std::abs(RuleLevelStateData.StartCell->GetPosition().x - PotentialGoalCell->GetPosition().x) > std::abs(RuleLevelStateData.StartCell->GetPosition().y - PotentialGoalCell->GetPosition().y))
			{
				MovePositionX += DirectionToGridStepX.at(Directions.GetHorizontal());
			}
			else if (std::abs(RuleLevelStateData.StartCell->GetPosition().y - PotentialGoalCell->GetPosition().y) > std::abs(RuleLevelStateData.StartCell->GetPosition().x - PotentialGoalCell->GetPosition().x))
			{
				MovePositionY += DirectionToGridStepY.at(Directions.GetVertical());
			}
		}

		return &RuleLevelStateData.LevelGrid[(PositionX + MovePositionX % RuleLevelStateData.GridWidth + RuleLevelStateData.GridWidth) % RuleLevelStateData.GridWidth][(PositionY + MovePositionY % RuleLevelStateData.GridHeight + RuleLevelStateData.GridHeight) % RuleLevelStateData.GridHeight];
	}

	std::vector<std::vector<RuleLevelCell>>& LevelGenerator::GetLevelGrid()
	{
		return RuleLevelStateData.LevelGrid;
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

		const int StepsToGoalX = abs(RuleLevelStateData.StartCell->GetPosition().x - RuleLevelStateData.GoalCell->GetPosition().x);
		const int StepsToGoalY = abs(RuleLevelStateData.StartCell->GetPosition().y - RuleLevelStateData.GoalCell->GetPosition().y);
		const int TotalSteps = StepsToGoalX + StepsToGoalY;

		return std::make_tuple(
			static_cast<int>(static_cast<float>(TotalSteps) * MinPercentage),
			static_cast<int>(static_cast<float>(TotalSteps) * MaxPercentage)
		);
	}

	int LevelGenerator::GetGridWidth() const
	{
		return RuleLevelStateData.GridWidth;
	}

	int LevelGenerator::GetGridHeight() const
	{
		return RuleLevelStateData.GridHeight;
	}
}
