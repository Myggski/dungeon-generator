#include "HiResLevelStateData.h"

namespace LevelGenerator
{
	HiResLevelStateData::HiResLevelStateData()
		: GridWidth(std::numeric_limits<int>::min()),
		GridHeight(std::numeric_limits<int>::min()),
		bHasGeneratedLevel(false) {}

	bool HiResLevelStateData::HasGeneratedLevel() const
	{
		return bHasGeneratedLevel;
	}
}