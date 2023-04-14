#include "CreateLowResLevelCommand.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelCell.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"

namespace Command
{
	CreateLowResLevelCommand::CreateLowResLevelCommand(LevelProcessState& CurrentProcessState, LevelGenerator::LowResLevelStateData& LevelData, LevelGenerator::RuleLevelStateData& RuleLevelStateData)
		: LevelData(LevelData),
		RuleLevelStateData(RuleLevelStateData),
		CurrentProcessState(CurrentProcessState) { }

	void CreateLowResLevelCommand::Execute()
	{
		LevelData.GridWidth = (RuleLevelStateData.GridWidth * 2) - 1;
		LevelData.GridHeight = (RuleLevelStateData.GridHeight * 2) - 1;
		LevelData.LowResGrid.resize(LevelData.GridWidth);

		for (int X = 0; X < LevelData.GridWidth; X++)
		{
			LevelData.LowResGrid[X].resize(LevelData.GridHeight);
		}

		for (int X = 0; X < RuleLevelStateData.GridWidth; X++) {
			for (int Y = 0; Y < RuleLevelStateData.GridHeight; Y++) {
				const int PositionX = X * 2;
				const int PositionY = Y * 2;

				LevelGenerator::RuleLevelCell* CurrentRuleLevelCell = &RuleLevelStateData.LevelGrid[X][Y];
				LevelGenerator::LowResCell* CurrentLowResCell = &LevelData.LowResGrid[PositionX][PositionY];

				*CurrentLowResCell = LevelGenerator::LowResCell(std::make_tuple(PositionX, PositionY), LevelGenerator::LowResCellType::Room, CurrentRuleLevelCell->GetElements());
				CurrentLowResCell->SetEntrance(CurrentRuleLevelCell->GetEntranceFlag());

				if (RuleLevelStateData.IsStartCell(CurrentRuleLevelCell))
				{
					LevelData.StartCell = CurrentLowResCell;
				}
				else if (RuleLevelStateData.IsGoalCell(CurrentRuleLevelCell))
				{
					LevelData.GoalCell = CurrentLowResCell;
				}

				if (CurrentRuleLevelCell->HasEntrance(DirectionType::East))
				{
					LevelData.LowResGrid[PositionX + 1][PositionY] = LevelGenerator::LowResCell(std::make_tuple(PositionX + 1, PositionY), LevelGenerator::LowResCellType::Entrance);
				}

				if (CurrentRuleLevelCell->HasEntrance(DirectionType::South))
				{
					LevelData.LowResGrid[PositionX][PositionY + 1] = LevelGenerator::LowResCell(std::make_tuple(PositionX, PositionY + 1), LevelGenerator::LowResCellType::Entrance);
				}
			}
		}

		CurrentProcessState = LevelProcessState::LowResLevel;
		LevelData.bHasGeneratedLevel = true;
	}

	void CreateLowResLevelCommand::Undo()
	{
		LevelData.LowResGrid.clear();
		LevelData.GridWidth = std::numeric_limits<int>::min();
		LevelData.GridHeight = std::numeric_limits<int>::min();
		LevelData.bHasGeneratedLevel = false;

		CurrentProcessState = LevelProcessState::RuleGridLevel;
	}
}