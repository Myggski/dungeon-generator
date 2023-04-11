#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"
#include "LevelGeneration/LevelGenerator/LowResLevel/LowResLevelStateData.h"

namespace Command
{
	class CreateLowResLevelCommand final : public Command
	{
	public:
		CreateLowResLevelCommand(LevelGenerator::LowResLevelStateData& LevelData, LevelGenerator::RuleLevelStateData& RuleLevelStateData);

		void Execute() override;
		void Undo() override;

	private:
		LevelGenerator::LowResLevelStateData& LevelData;
		LevelGenerator::RuleLevelStateData& RuleLevelStateData;
	};
}