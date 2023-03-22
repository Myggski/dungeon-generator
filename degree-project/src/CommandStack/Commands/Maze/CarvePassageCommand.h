#pragma once

#include "CarvePassageCommandData.h"
#include "CommandStack/Commands/Command.h"

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
		CarvePassageCommand(CarvePassageCommandData Data);
		~CarvePassageCommand() override;

		void Execute() override;
		void Undo() override;

	private:
		CarvePassageCommandData Data;
	};
}
