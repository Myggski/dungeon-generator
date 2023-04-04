#include "AddSideRoomsCommand.h"

#include "LevelGeneration/LevelGenerator/LevelCell.h"
#include "LevelGeneration/LevelGenerator/LevelGenerator.h"
#include "LevelGeneration/LevelGenerator/LevelStateData.h"

Command::AddSideRoomsCommand::AddSideRoomsCommand(LevelGenerator::LevelStateData& StateData)
	: StateData(StateData) { }

void Command::AddSideRoomsCommand::Execute()
{
	while (StateData.CurrentInsertionIndex < 1)
	{
		for (LevelGenerator::LevelCell* Cell : StateData.GetPathway(StateData.CurrentInsertionIndex))
		{
			auto Directions = StateData.GetAvailableDirections(Cell);

			if (!Directions.empty())
			{
				const auto* SideRoomCell = StateData.GetNeighborCell(Cell, Directions[0]);
				Cell->CarveEntrance(SideRoomCell->GetSpatialHash());
				StateData.LevelGrid[SideRoomCell->GetPosition().x][SideRoomCell->GetPosition().y].CarveEntrance(Cell->GetSpatialHash());
				StateData.LevelGrid[SideRoomCell->GetPosition().x][SideRoomCell->GetPosition().y].SetVisited(true);
				StateData.GetPathway(StateData.CurrentInsertionIndex).push_back(Cell);

				if (StateData.CurrentInsertionIndex < 2)
				{
					StateData.CurrentInsertionIndex++;
					break;
				}
			}
		}
	}

	StateData.CurrentAction = LevelGenerator::GeneratorActionType::Done;
}

void Command::AddSideRoomsCommand::Undo()
{
	
}
