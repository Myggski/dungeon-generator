#pragma once

#include <vector>
#include "LevelGeneration/LevelGenerator/HiResLevel/Room.h"

namespace Command
{
	class CreateHiResLevelCommand;
}

namespace LevelGeneration
{
	class Level;
}

namespace LevelGenerator
{
	struct HiResLevelStateData
	{
	public:
		HiResLevelStateData(std::tuple<float, float> RoomCellSize);

		bool HasGeneratedLevel() const;

	private:
		int GridWidth;
		int GridHeight;
		bool bHasGeneratedLevel;
		std::tuple<float, float> RoomCellSize;
		std::vector<std::vector<LevelGeneration::Room>> HiResGrid;
		LevelGeneration::Room* StartRoom;
		LevelGeneration::Room* GoalRoom;

		friend class LevelGeneration::Level;
		friend class Command::CreateHiResLevelCommand;
	};
}