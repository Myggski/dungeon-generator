#include "LowResLevelStateData.h"

namespace LevelGenerator {
	LowResLevelStateData::LowResLevelStateData()
		: GridSizeWidth(std::numeric_limits<int>::min()),
		GridSizeHeight(std::numeric_limits<int>::min()),
		bHasGeneratedLevel(false) {}

	bool LowResLevelStateData::HasGeneratedLevel() const
	{
		return bHasGeneratedLevel;
	}
}