#include "CreateHiResLevelCommand.h"

#include "LevelGeneration/LevelGenerator/HiResLevel/Room.h"

namespace Command
{
	CreateHiResLevelCommand::CreateHiResLevelCommand(LevelProcessState& CurrentProcessState, LevelGenerator::HiResLevelStateData& LevelData, LevelGenerator::LowResLevelStateData& LowResLevelStateData)
		: CurrentProcessState(CurrentProcessState),
		LevelData(LevelData),
		LowResLevelStateData(LowResLevelStateData) {}

	void CreateHiResLevelCommand::Execute()
	{
		const float RoomSize = 7.0f;
		LevelData.GridWidth = (LowResLevelStateData.GridWidth + 1) / 2;
		LevelData.GridHeight = (LowResLevelStateData.GridHeight + 1) / 2;
		LevelData.HiResGrid.resize(LevelData.GridWidth);

		for (int X = 0; X < LevelData.GridWidth; X++)
		{
			LevelData.HiResGrid[X].resize(LevelData.GridHeight);
		}

		for (int X = 0; X < LowResLevelStateData.GridWidth; X++)
		{
			for (int Y = 0; Y < LowResLevelStateData.GridHeight; Y++)
			{
				const float HiResX = X / 2.0;
				const float HiResY = Y / 2.0;
				LevelGenerator::LowResCellType CellType = LowResLevelStateData.LowResGrid[X][Y].GetType();

				if (CellType == LevelGenerator::LowResCellType::Room)
				{
					LevelData.HiResGrid[static_cast<int>(HiResX)][static_cast<int>(HiResY)] = LevelGeneration::Room(
						SDL_FRect{ HiResX * RoomSize, HiResY * RoomSize, RoomSize, RoomSize }
					);

					if (X < LowResLevelStateData.GridWidth - 1)
					{
						const LevelGenerator::LowResCell& EastNeighbor = LowResLevelStateData.LowResGrid[X + 1][Y];
						// Add Entrance
						if (EastNeighbor.GetType() == LevelGenerator::LowResCellType::Entrance)
						{

						}
					}

					if (Y < LowResLevelStateData.GridHeight - 1)
					{
						const LevelGenerator::LowResCell& SouthNeighbor = LowResLevelStateData.LowResGrid[X][Y + 1];
						if (SouthNeighbor.GetType() == LevelGenerator::LowResCellType::Entrance)
						{

						}
					}
				}
			}
		}

		CurrentProcessState = LevelProcessState::Done;
		LevelData.bHasGeneratedLevel = true;
	}

	void CreateHiResLevelCommand::Undo()
	{
		LevelData.HiResGrid.clear();
		LevelData.GridWidth = std::numeric_limits<int>::min();
		LevelData.GridHeight = std::numeric_limits<int>::min();
		LevelData.bHasGeneratedLevel = false;

		CurrentProcessState = LevelProcessState::LowResLevel;
	}
}