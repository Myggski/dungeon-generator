#pragma once

#include <vector>
#include "LowResCell.h"

namespace LevelGeneration
{
	class Level;
}

namespace Command
{
	class CreateLowResLevelCommand;
}

namespace LevelGenerator
{
	struct LowResLevelStateData
	{
	public:
		LowResLevelStateData();

		bool HasGeneratedLevel() const;

	private:
		int GridSizeWidth;
		int GridSizeHeight;
		bool bHasGeneratedLevel;
		std::vector<std::vector<LowResCell>> LowResGrid;

		friend class LevelGeneration::Level;
		friend class Command::CreateLowResLevelCommand;

	};
}