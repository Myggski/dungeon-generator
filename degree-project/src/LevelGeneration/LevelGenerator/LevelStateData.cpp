#include "LevelStateData.h"

#include "DirectionType.h"

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
}
