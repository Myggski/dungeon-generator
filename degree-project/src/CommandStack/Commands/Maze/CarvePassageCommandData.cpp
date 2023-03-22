#include "CarvePassageCommandData.h"

namespace Command
{
	CarvePassageCommandData::CarvePassageCommandData(
		MazeGenerator::MazeCell* CarveFromCell,
		MazeGenerator::MazeCell* CarveToCell,
		DirectionType MoveTowardsDirection,
		DirectionType PreviousDirection,
		std::stack<MazeGenerator::MazeCell*>& VisitedStack,
		std::function<void(MazeGenerator::MazeCell*)> SetCurrentCellCallback
	) : MoveTowardsDirection(MoveTowardsDirection),
	    PreviousDirection(PreviousDirection),
		CarveFromCell(CarveFromCell),
		CarveToCell(CarveToCell),
		VisitedStack(VisitedStack),
		SetCurrentCellCallback(std::move(SetCurrentCellCallback)) { }
}
