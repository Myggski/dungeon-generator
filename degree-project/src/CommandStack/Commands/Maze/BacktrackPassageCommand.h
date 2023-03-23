#pragma once

#include <cstdint>

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

	class BacktrackPassageCommand final : public Command
	{
	public:
		BacktrackPassageCommand(MazeGenerator::MazeStateData& StateData);

		void Execute() override;
		void Undo() override;

	private:
		MazeGenerator::MazeStateData& StateData;
		MazeGenerator::MazeCell* PreviousCell;
	};
}
