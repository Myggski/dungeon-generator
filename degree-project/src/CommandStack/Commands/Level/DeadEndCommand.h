#pragma once

#include <cstdint>

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"

enum class DirectionType : uint8_t;

namespace LevelGenerator
{
	class RuleLevelGenerator;
	class RuleLevelCell;
}

namespace Command
{

	class DeadEndCommand final : public Command
	{
	public:
		DeadEndCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData);

		void Execute() override;
		void Undo() override;

	private:
		LevelGenerator::RuleLevelStateData& RuleLevelStateData;
		LevelGenerator::RuleLevelCell* DeadEndCell;
	};
}
