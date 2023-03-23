#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/MazeGenerator/MazeStateData.h"

enum class DirectionType : uint8_t;

namespace MazeGenerator
{
	class Maze;
	class MazeCell;
}

namespace Command
{

	class CarvePassageCommand final : public Command
	{
	public:
		CarvePassageCommand(MazeGenerator::MazeStateData& StateData, DirectionType MoveTowardsDirection);
		~CarvePassageCommand() override;

		void Execute() override;
		void Undo() override;

	private:
		MazeGenerator::MazeStateData& StateData;
		MazeGenerator::MazeCell* CarvedFromCell;
		MazeGenerator::MazeCell* CarvedToCell;
		DirectionType MoveTowardsDirection;
		MazeGenerator::MazeActionType PreviousAction;
	};
}
