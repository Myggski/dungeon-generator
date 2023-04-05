#pragma once
#include "LevelGeneration/LevelGenerator/LevelGenerator.h"

namespace LevelGenerator
{
	class LevelLowRes
	{
	public:
		LevelLowRes(LevelGenerator& LevelGenerator);

	private:
		int GridSizeX;
		int GridSizeY;
		std::vector<std::vector<LevelCell>> LowResGrid;
	};
}
