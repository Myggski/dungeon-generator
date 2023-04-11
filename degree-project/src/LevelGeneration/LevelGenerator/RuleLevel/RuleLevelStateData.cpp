#include "RuleLevelStateData.h"

#include <numeric>

#include "LevelGeneration/LevelGenerator/RuleLevel/DirectionType.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/NavigationalDirections.h"

namespace LevelGenerator
{
	RuleLevelStateData::RuleLevelStateData(int GridWidth, int GridHeight)
		: GridWidth(GridWidth),
		GridHeight(GridHeight),
		CurrentInsertionIndex(-1),
		CurrentCell(nullptr),
		StartCell(nullptr),
		GoalCell(nullptr),
		CurrentAction(GeneratorActionType::None) { }

	std::vector<RuleLevelCell*>& RuleLevelStateData::GetCurrentPathway()
	{
		return InsertionPathways[CurrentInsertionIndex];
	}

	std::vector<RuleLevelCell*>& RuleLevelStateData::GetPathway(int InsertionIndex)
	{
		return InsertionPathways[InsertionIndex];
	}

	void RuleLevelStateData::AddCellToPathway(RuleLevelCell* Cell)
	{
		InsertionPathways[CurrentInsertionIndex].emplace_back(Cell);
	}

	void RuleLevelStateData::RemoveLastCellFromPathway()
	{
		InsertionPathways[CurrentInsertionIndex].erase(InsertionPathways[CurrentInsertionIndex].end() - 1);
	}

	bool RuleLevelStateData::IsPathwayEmpty() const
	{
		return InsertionPathways[CurrentInsertionIndex].empty();
	}

	bool RuleLevelStateData::IsStartCell(RuleLevelCell* Cell) const
	{
		return Cell == StartCell;
	}

	bool RuleLevelStateData::IsGoalCell(RuleLevelCell* Cell) const
	{
		return Cell == GoalCell;
	}

	void RuleLevelStateData::ReversePathway(int InsertionIndex)
	{
		std::ranges::reverse(InsertionPathways[InsertionIndex]);
	}

	void RuleLevelStateData::AddPreviousDirection(DirectionType Direction)
	{
		if (PreviousDirections.size() > 1)
		{
			PreviousDirections.pop();
		}

		PreviousDirections.push(Direction);
	}

	int RuleLevelStateData::GetNumberOfEmptySlots()
	{
		int NumberOfEmptySlots = 0;

		for (int X = 0; X < GridWidth; X++)
		{
			for (int Y = 0; Y < GridHeight; Y++)
			{
				if (!LevelGrid[X][Y].IsVisited())
				{
					NumberOfEmptySlots++;
				}
			}
		}

		return NumberOfEmptySlots;
	}

	std::vector<DirectionType> RuleLevelStateData::GetAvailableDirections(RuleLevelCell* Cell)
	{
		std::vector<DirectionType> AvailableDirections;
		AvailableDirections.reserve(4);

		const std::array<DirectionType, 4> Directions = NavigationalDirections::GetDirections();

		for (const auto& CurrentDirection : Directions)
		{
			const RuleLevelCell* Neighbor = GetNeighborCell(Cell, CurrentDirection);

			if (Neighbor == nullptr || Neighbor->IsVisited())
			{
				continue;
			}

			AvailableDirections.emplace_back(CurrentDirection);
		}

		return AvailableDirections;
	}

	RuleLevelCell* RuleLevelStateData::GetNeighborCell(RuleLevelCell* From, DirectionType Direction)
	{
		const int NeighborX = From->GetPosition().x + DirectionToGridStepX.at(Direction);
		const int NeighborY = From->GetPosition().y + DirectionToGridStepY.at(Direction);

		if (IsOutOfBound(NeighborX, NeighborY))
		{
			return nullptr;
		}

		return &LevelGrid[NeighborX][NeighborY];
	}

	bool RuleLevelStateData::IsOutOfBound(int PositionX, int PositionY) const
	{
		return (PositionX < 0 || PositionX > GridWidth - 1) || (PositionY < 0 || PositionY > GridHeight - 1);
	}
}
