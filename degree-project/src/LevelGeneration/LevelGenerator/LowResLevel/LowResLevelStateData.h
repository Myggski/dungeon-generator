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
	class CreateHiResLevelCommand;
}

namespace LevelGenerator
{
	struct LowResLevelStateData
	{
	public:
		LowResLevelStateData();

		bool HasGeneratedLevel() const;

	private:
		int GridWidth;
		int GridHeight;
		bool bHasGeneratedLevel;
		LowResCell* StartCell;
		LowResCell* GoalCell;
		std::vector<std::vector<LowResCell>> LowResGrid;

		friend class LevelGeneration::Level;
		friend class Command::CreateLowResLevelCommand;
		friend class Command::CreateHiResLevelCommand;

	};
}