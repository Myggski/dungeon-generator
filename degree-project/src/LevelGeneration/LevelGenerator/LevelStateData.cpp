#include "LevelStateData.h"

#include <numeric>

#include "DirectionType.h"
#include "NavigationalDirections.h"

namespace LevelGenerator
{
	LevelStateData::LevelStateData(int GridWidth, int GridHeight)
		: GridWidth(GridWidth),
		GridHeight(GridHeight),
		CurrentInsertionIndex(-1),
		CurrentCell(nullptr),
		StartCell(nullptr),
		GoalCell(nullptr),
		CurrentAction(GeneratorActionType::None) { }

	std::vector<LevelCell*>& LevelStateData::GetCurrentPathway()
	{
		return InsertionPathways[CurrentInsertionIndex];
	}

	std::vector<LevelCell*>& LevelStateData::GetPathway(int InsertionIndex)
	{
		return InsertionPathways[InsertionIndex];
	}

	void LevelStateData::AddCellToPathway(LevelCell* Cell)
	{
		InsertionPathways[CurrentInsertionIndex].emplace_back(Cell);
	}

	void LevelStateData::RemoveLastCellFromPathway()
	{
		InsertionPathways[CurrentInsertionIndex].erase(InsertionPathways[CurrentInsertionIndex].end() - 1);
	}

	bool LevelStateData::IsPathwayEmpty() const
	{
		return InsertionPathways[CurrentInsertionIndex].empty();
	}

	void LevelStateData::ReversePathway(int InsertionIndex)
	{
		std::ranges::reverse(InsertionPathways[InsertionIndex]);
	}

	void LevelStateData::AddPreviousDirection(DirectionType Direction)
	{
		if (PreviousDirections.size() > 1)
		{
			PreviousDirections.pop();
		}

		PreviousDirections.push(Direction);
	}

	int LevelStateData::GetNumberOfEmptySlots()
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
	
	std::vector<DirectionType> LevelStateData::GetAvailableDirections(LevelCell* Cell)
	{
		std::vector<DirectionType> AvailableDirections;
		AvailableDirections.reserve(4);

		const std::array<DirectionType, 4> Directions = NavigationalDirections::GetDirections();

		for (const auto& CurrentDirection : Directions)
		{
			const LevelCell* Neighbor = GetNeighborCell(Cell, CurrentDirection);

			if (Neighbor == nullptr || Neighbor->IsVisited())
			{
				continue;
			}

			AvailableDirections.emplace_back(CurrentDirection);
		}

		return AvailableDirections;
	}

	LevelCell* LevelStateData::GetNeighborCell(LevelCell* From, DirectionType Direction)
	{
		const int NeighborX = From->GetPosition().x + DirectionToGridStepX.at(Direction);
		const int NeighborY = From->GetPosition().y + DirectionToGridStepY.at(Direction);

		if (IsOutOfBound(NeighborX, NeighborY))
		{
			return nullptr;
		}

		return &LevelGrid[NeighborX][NeighborY];
	}

	bool LevelStateData::IsOutOfBound(int PositionX, int PositionY) const
	{
		return (PositionX < 0 || PositionX > GridWidth - 1) || (PositionY < 0 || PositionY > GridHeight - 1);
	}
}
