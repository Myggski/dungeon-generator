#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/LowResLevel/LowResLevelStateData.h"
#include "LevelGeneration/LevelGenerator/HiResLevel/HiResLevelStateData.h"
#include "LevelGeneration/LevelGenerator/ProcessState.h"

namespace Command
{
	class CreateHiResLevelCommand final : public Command
	{
	public:
		CreateHiResLevelCommand(LevelProcessState& CurrentProcessState, LevelGenerator::HiResLevelStateData& LevelData, LevelGenerator::LowResLevelStateData& LowResLevelStateData);

		void Execute() override;
		void Undo() override;

	private:
		LevelProcessState& CurrentProcessState;
		LevelGenerator::HiResLevelStateData& LevelData;
		LevelGenerator::LowResLevelStateData& LowResLevelStateData;
	};
}