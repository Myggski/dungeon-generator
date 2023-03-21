#include "CarvePassageCommandData.h"

namespace Command
{
	CarvePassageCommandData::CarvePassageCommandData()
		: MoveTowardsDirection(DirectionType::None),
		  PreviousDirection(DirectionType::None),
		  CarveFromCell(nullptr),
		  CarveToCell(nullptr),
		  SetCurrentCellCallback(std::nullopt) { }

	CarvePassageCommandData::CarvePassageCommandData(
		MazeGenerator::MazeCell* CarveFromCell, 
		MazeGenerator::MazeCell* CarveToCell, 
		DirectionType MoveTowardsDirection, 
		DirectionType PreviousDirection,
		const std::function<void(MazeGenerator::MazeCell*)>& SetCurrentCellCallback
	) : MoveTowardsDirection(MoveTowardsDirection),
	    PreviousDirection(PreviousDirection),
		CarveFromCell(CarveFromCell),
		CarveToCell(CarveToCell),
		SetCurrentCellCallback(std::optional<std::function<void(MazeGenerator::MazeCell*)>>(SetCurrentCellCallback)) { }
}
