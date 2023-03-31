#pragma once

#include <cstdint>

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/LevelStateData.h"

enum class DirectionType : uint8_t;

namespace LevelGenerator
{
	class LevelGenerator;
	class LevelCell;
}

namespace Command
{

	class BacktrackPassageCommand final : public Command
	{
	public:
		BacktrackPassageCommand(LevelGenerator::LevelStateData& StateData);

		void Execute() override;
		void Undo() override;

	private:
		bool PathReversed;
		LevelGenerator::LevelStateData& StateData;
		LevelGenerator::LevelCell* PreviousCell;
	};
}
