#pragma once

#include <cstdint>

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"

enum class DirectionType : uint8_t;

namespace LevelGenerator
{
	class LevelGenerator;
	class RuleLevelCell;
}

namespace Command
{

	class BacktrackPassageCommand final : public Command
	{
	public:
		BacktrackPassageCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData);

		void Execute() override;
		void Undo() override;

	private:
		bool PathReversed;
		LevelGenerator::RuleLevelStateData& RuleLevelStateData;
		LevelGenerator::RuleLevelCell* PreviousCell;
	};
}
