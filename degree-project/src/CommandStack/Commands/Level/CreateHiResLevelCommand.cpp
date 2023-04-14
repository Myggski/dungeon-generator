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
				LevelGenerator::LowResCell* CurrentLowResCell = &LowResLevelStateData.LowResGrid[X][Y];
				LevelGenerator::LowResCellType CellType = CurrentLowResCell->GetType();

				if (CellType == LevelGenerator::LowResCellType::Room)
				{
					LevelGeneration::Room* CurrentRoom = &LevelData.HiResGrid[static_cast<int>(HiResX)][static_cast<int>(HiResY)];
					*CurrentRoom = LevelGeneration::Room(
						SDL_FRect{ HiResX * RoomSize, HiResY * RoomSize, RoomSize, RoomSize }
					);

					if (LowResLevelStateData.StartCell == CurrentLowResCell)
					{
						LevelData.StartRoom = CurrentRoom;
					}

					if (LowResLevelStateData.GoalCell == CurrentLowResCell)
					{
						LevelData.GoalRoom = CurrentRoom;
					}

					if (CurrentLowResCell->HasEntrance(DirectionType::East))
					{

					}

					if (CurrentLowResCell->HasEntrance(DirectionType::West))
					{

					}

					if (CurrentLowResCell->HasEntrance(DirectionType::North))
					{

					}

					if (CurrentLowResCell->HasEntrance(DirectionType::South))
					{

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