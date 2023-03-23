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

	class CreateNewPathCommand final : public Command
	{
	public:
		CreateNewPathCommand(MazeGenerator::MazeStateData& StateData, std::array<DirectionType, 4> RandomDirections);
		~CreateNewPathCommand() override;

		void Execute() override;
		void Undo() override;

	private:
		static bool IsOutOfBound(const MazeGenerator::MazeStateData& StateData, int PositionX, int PositionY)
		{
			return (PositionX < 0 || PositionX > StateData.GridWidth - 1) || (PositionY < 0 || PositionY > StateData.GridHeight - 1);
		}

	private:
		MazeGenerator::MazeStateData& StateData;
		std::vector<MazeGenerator::MazeCell*> ChangedCells;
		std::array<DirectionType, 4> RandomDirections;
	};
}
