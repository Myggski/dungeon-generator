#include "LowResLevelStateData.h"

namespace LevelGenerator {
	LowResLevelStateData::LowResLevelStateData()
		: GridWidth(std::numeric_limits<int>::min()),
		GridHeight(std::numeric_limits<int>::min()),
		bHasGeneratedLevel(false) {}

	bool LowResLevelStateData::HasGeneratedLevel() const
	{
		return bHasGeneratedLevel;
	}
}