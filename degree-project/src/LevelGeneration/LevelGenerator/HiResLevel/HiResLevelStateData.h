#pragma once

#include <vector>
#include "LevelGeneration/Room.h"

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
		HiResLevelStateData();

		bool HasGeneratedLevel() const;

	private:
		int GridWidth;
		int GridHeight;
		bool bHasGeneratedLevel;
		std::vector<std::vector<LevelGeneration::Room>> HiResGrid;

		friend class LevelGeneration::Level;
		friend class Command::CreateHiResLevelCommand;
	};
}