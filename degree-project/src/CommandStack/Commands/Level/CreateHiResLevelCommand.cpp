#include "CreateHiResLevelCommand.h"

namespace Command
{
	CreateHiResLevelCommand::CreateHiResLevelCommand(LevelProcessState& CurrentProcessState, LevelGenerator::HiResLevelStateData& LevelData, LevelGenerator::LowResLevelStateData& LowResLevelStateData)
		: CurrentProcessState(CurrentProcessState),
		LevelData(LevelData),
		LowResLevelStateData(LowResLevelStateData) {}

	void CreateHiResLevelCommand::Execute()
	{
		LevelData.GridWidth = (LowResLevelStateData.GridWidth + 1) / 2.f;
		LevelData.GridHeight = (LowResLevelStateData.GridHeight + 1) / 2.f;
		LevelData.HiResGrid.resize(LevelData.GridWidth);

		for (int X = 0; X < LevelData.GridWidth; X++)
		{
			LevelData.HiResGrid[X].resize(LevelData.GridHeight);
		}
	}

	void CreateHiResLevelCommand::Undo()
	{
		LevelData.HiResGrid.clear();
		LevelData.GridWidth = std::numeric_limits<int>::min();
		LevelData.GridHeight = std::numeric_limits<int>::min();
		LevelData.bHasGeneratedLevel = false;

		CurrentProcessState = LevelProcessState::LowResLevel;
	}
}