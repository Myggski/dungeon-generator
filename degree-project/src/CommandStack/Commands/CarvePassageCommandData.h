#pragma once
#include <functional>
#include <optional>

#include "LevelGeneration/MazeGenerator/DirectionType.h"
#include "LevelGeneration/MazeGenerator/MazeCell.h"

namespace Command
{
	class CarvePassageCommand;

	struct CarvePassageCommandData
	{
	public:
		CarvePassageCommandData();
		CarvePassageCommandData(MazeGenerator::MazeCell* CarveFromCell,
			MazeGenerator::MazeCell* CarveToCell,
			DirectionType MoveTowardsDirection,
			DirectionType PreviousDirection,
			const std::function<void(MazeGenerator::MazeCell*)>& SetCurrentCellCallback);

	private:
		DirectionType MoveTowardsDirection;
		DirectionType PreviousDirection;
		MazeGenerator::MazeCell* CarveFromCell;
		MazeGenerator::MazeCell* CarveToCell;
		std::optional<std::function<void(MazeGenerator::MazeCell*)>> SetCurrentCellCallback;

		friend CarvePassageCommand;
	};
}
