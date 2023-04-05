#include "LevelLowRes.h"

namespace LevelGenerator
{
	LevelLowRes::LevelLowRes(LevelGenerator& LevelGenerator)
		: GridSizeX((LevelGenerator.GetGridWidth() * 2) - 1),
		GridSizeY((LevelGenerator.GetGridHeight() * 2) - 1)
	{
		LowResGrid.resize(GridSizeX * GridSizeY);

		for (int X = 0; X < LevelGenerator.GetGridWidth(); X++) {
			for (int Y = 0; Y < LevelGenerator.GetGridHeight(); Y++) {
				LowResGrid.at(X * 2).at(Y * 2) = LevelGenerator.GetLevelGrid()[X][Y];
			}
		}
	}
}