#include "LowResLevel.h"

namespace LevelGenerator
{
	LowResLevel::LowResLevel(LevelGenerator& LevelGenerator)
		: GridSizeWidth((LevelGenerator.GetGridWidth() * 2) - 1),
		GridSizeHeight((LevelGenerator.GetGridHeight() * 2) - 1)
	{
		LowResGrid.resize(GridSizeWidth);

		for (int X = 0; X < GridSizeWidth; X++)
		{
			LowResGrid[X].resize(GridSizeHeight);
		}

		for (int X = 0; X < LevelGenerator.GetGridWidth(); X++) {
			for (int Y = 0; Y < LevelGenerator.GetGridHeight(); Y++) {
				int PositionX = X * 2;
				int PositionY = Y * 2;
				LevelCell* Cell = &LevelGenerator.GetLevelGrid()[X][Y];

				LowResGrid[PositionX][PositionY] = LowResCell(std::make_tuple(PositionX, PositionY), LowResCellType::Room);

				if (Cell->HasEntrance(DirectionType::East))
				{
					LowResGrid[PositionX + 1][PositionY] = LowResCell(std::make_tuple(PositionX + 1, PositionY), LowResCellType::Entrance);
				}
				
				if (Cell->HasEntrance(DirectionType::South))
				{
					LowResGrid[PositionX][PositionY + 1] = LowResCell(std::make_tuple(PositionX, PositionY + 1), LowResCellType::Entrance);
				}
			}
		}
	}

	const std::vector<std::vector<LowResCell>>& LowResLevel::GetGrid() const
	{
		return LowResGrid;
	}

	int LowResLevel::GetGridWidth() const
	{
		return GridSizeWidth;
	}

	int LowResLevel::GetGridHeight() const
	{
		return GridSizeWidth;
	}
}