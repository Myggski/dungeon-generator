#pragma once
#include "LevelGeneration/LevelGenerator/LevelGenerator.h"
#include "LowResCell.h"

namespace LevelGenerator
{
	struct LowResLevel
	{
	public:
		LowResLevel(LevelGenerator& LevelGenerator);

		const std::vector<std::vector<LowResCell>>& GetGrid() const;
		int GetGridWidth() const;
		int GetGridHeight() const;

	private:
		int GridSizeWidth;
		int GridSizeHeight;
		std::vector<std::vector<LowResCell>> LowResGrid;
	};
}
