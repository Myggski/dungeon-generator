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

		inline int GetWidth() const
		{
			return GridWidth;
		}

		inline int GetHeight() const
		{
			return GridHeight;
		}

		inline bool IsStartCell(int X, int Y) const
		{
			if (StartCell == nullptr)
			{
				return false;
			}

			auto [CellX, CellY] = StartCell->GetPosition();
			return X == CellX && Y == CellY;
		}

		inline LowResCell* GetCell(int X, int Y)
		{
			return &LowResGrid[X][Y];
		}

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