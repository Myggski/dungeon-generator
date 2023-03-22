#pragma once

#include <cstdint>
#include <functional>
#include <stack>
#include <vector>

#include "CommandStack/Commands/Command.h"

enum class DirectionType : uint8_t;

namespace MazeGenerator
{
	class Maze;
	class MazeCell;
}

namespace Command
{

	class BacktrackPassageCommand final : public Command
	{
	public:
		BacktrackPassageCommand(MazeGenerator::MazeCell* CurrentCell, std::stack<MazeGenerator::MazeCell*>& VisitedStack, std::vector<MazeGenerator::MazeCell*>& Pathway, bool bGoalHasBeenReached, std::function<void(MazeGenerator::MazeCell*)> SetCurrentCellCallback);

		void Execute() override;
		void Undo() override;

	private:
		bool bGoalHasBeenReached;
		MazeGenerator::MazeCell* CurrentCell;
		MazeGenerator::MazeCell* PreviousCell;
		std::stack<MazeGenerator::MazeCell*>& VisitedStack;
		std::vector<MazeGenerator::MazeCell*>& Pathway;
		std::function<void(MazeGenerator::MazeCell*)> SetCurrentCellCallback;
	};
}
