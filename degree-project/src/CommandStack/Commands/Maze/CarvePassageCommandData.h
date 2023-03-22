#pragma once

#include <functional>
#include <stack>

#include "LevelGeneration/MazeGenerator/DirectionType.h"
#include "LevelGeneration/MazeGenerator/MazeCell.h"

namespace Command
{
	class CarvePassageCommand;

	struct CarvePassageCommandData
	{
	public:
		CarvePassageCommandData(MazeGenerator::MazeCell* CarveFromCell,
			MazeGenerator::MazeCell* CarveToCell,
			DirectionType MoveTowardsDirection,
			DirectionType PreviousDirection,
			std::stack<MazeGenerator::MazeCell*>& VisitedStack,
			std::function<void(MazeGenerator::MazeCell*)> SetCurrentCellCallback);

	private:
		DirectionType MoveTowardsDirection;
		DirectionType PreviousDirection;
		MazeGenerator::MazeCell* CarveFromCell;
		MazeGenerator::MazeCell* CarveToCell;
		std::stack<MazeGenerator::MazeCell*>& VisitedStack;
		std::function<void(MazeGenerator::MazeCell*)> SetCurrentCellCallback;

		friend CarvePassageCommand;
	};
}
