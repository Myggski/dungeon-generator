#include "CreateHiResLevelCommand.h"

#include <sstream>

#include "LevelGeneration/LevelGenerator/HiResLevel/Room.h"

namespace Command
{
	CreateHiResLevelCommand::CreateHiResLevelCommand(LevelProcessState& CurrentProcessState, LevelGenerator::HiResLevelStateData& LevelData, LevelGenerator::LowResLevelStateData& LowResLevelStateData)
		: CurrentProcessState(CurrentProcessState),
		LevelData(LevelData),
		LowResLevelStateData(LowResLevelStateData) {}

	std::string CoordinatesToSpatialHash(int X, int Y)
	{
		std::stringstream SpatialHashStream;
		SpatialHashStream << X << "." << Y;
		return SpatialHashStream.str();
	}

	void CreateHiResLevelCommand::Execute()
	{
		const float RoomSize = 6.0f;
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
					
					CurrentRoom->SetEntrance(CurrentLowResCell->GetEntrance());
					CurrentRoom->SetType(CurrentLowResCell->GetRoomType());

					if (LowResLevelStateData.StartCell == CurrentLowResCell)
					{
						LevelData.StartRoom = CurrentRoom;
					}

					if (LowResLevelStateData.GoalCell == CurrentLowResCell)
					{
						LevelData.GoalRoom = CurrentRoom;
					}
				}
			}
		}

		for (int X = 0; X < LevelData.GridWidth; X++)
		{
			for (int Y = 0; Y < LevelData.GridHeight; Y++)
			{
				LevelGeneration::Room* CurrentRoom = &LevelData.HiResGrid[static_cast<int>(X)][static_cast<int>(Y)];
				if (CurrentRoom->GetType() == LevelGenerator::RoomType::Hub)
				{
					CurrentRoom->AdjustWidth(-2, X != LevelData.GridWidth - 1 ? 0 : 2);

					if (X != LevelData.GridWidth - 1)
					{
						LevelData.HiResGrid[static_cast<int>(X + 1)][static_cast<int>(Y)].AdjustWidth(2, -2);
					} 
					else 
					{
						LevelData.HiResGrid[static_cast<int>(X - 1)][static_cast<int>(Y)].AdjustWidth(2);
					}
				}

				if (CurrentRoom->GetType() == LevelGenerator::RoomType::Laundry || CurrentRoom->GetType() == LevelGenerator::RoomType::Communication)
				{
					CurrentRoom->AdjustWidth(-1, X != LevelData.GridWidth - 1 ? 0 : 1);
					if (X != LevelData.GridWidth - 1)
					{
						LevelData.HiResGrid[static_cast<int>(X + 1)][static_cast<int>(Y)].AdjustWidth(1, -1);
					}
					else
					{
						LevelData.HiResGrid[static_cast<int>(X - 1)][static_cast<int>(Y)].AdjustWidth(1);
					}
				}

				if (CurrentRoom->GetType() == LevelGenerator::RoomType::Bridge)
				{
					CurrentRoom->AdjustWidth(1, X != LevelData.GridWidth - 1 ? 0 : -1);
					if (X != LevelData.GridWidth - 1)
					{
						LevelData.HiResGrid[static_cast<int>(X + 1)][static_cast<int>(Y)].AdjustWidth(-1, 1);
					}
					else
					{
						LevelData.HiResGrid[static_cast<int>(X - 1)][static_cast<int>(Y)].AdjustWidth(-1);
					}
				}

				CurrentRoom->Elements.insert(std::make_pair(CoordinatesToSpatialHash((CurrentRoom->RoomRect.w / 2) - 1, (CurrentRoom->RoomRect.h / 2) - 1), LowResLevelStateData.LowResGrid[X * 2][Y * 2].GetElements()));
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