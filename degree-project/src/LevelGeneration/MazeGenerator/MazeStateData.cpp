#include "MazeStateData.h"

#include "DirectionType.h"

namespace MazeGenerator
{
	MazeStateData::MazeStateData(int GridWidth, int GridHeight)
		: GridWidth(GridWidth),
		GridHeight(GridHeight),
		CurrentPathwayIndex(0),
		CurrentCell(nullptr),
		StartCell(nullptr),
		GoalCell(nullptr),
		PreviousDirection(DirectionType::None),
		CurrentAction(MazeActionType::None) { }

	std::vector<MazeCell*>& MazeStateData::GetCurrentPathway()
	{
		return Pathways[CurrentPathwayIndex];
	}

	void MazeStateData::AddCellToPathway(MazeCell* Cell)
	{
		Pathways[CurrentPathwayIndex].emplace_back(Cell);
	}

	void MazeStateData::RemoveLastCellFromPathway()
	{
		Pathways[CurrentPathwayIndex].erase(Pathways[CurrentPathwayIndex].end() - 1);
	}

	bool MazeStateData::IsPathwayEmpty() const
	{
		return Pathways[CurrentPathwayIndex].empty();
	}
}
