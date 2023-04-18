#include "AddSideRoomsCommand.h"

namespace Command
{

	AddSideRoomsCommand::AddSideRoomsCommand(LevelGenerator::RuleLevelStateData& StateData, std::array<DirectionType, 4> RandomDirections, bool AddAdditionalRoom)
		: StateData(StateData),
		RandomDirections(RandomDirections),
		AddAdditionalRoom(AddAdditionalRoom),
		PreviousInsertionIndex(StateData.CurrentInsertionIndex) { }

	void AddSideRoomsCommand::Execute()
	{
		for (LevelGenerator::RuleLevelCell* Cell : StateData.GetPathway(StateData.CurrentInsertionIndex))
		{
			if (LevelGenerator::RuleLevelCell* SideRoom = TryGenerateSideRoom(Cell))
			{
				if (AddAdditionalRoom)
				{
					// And are able to generate another room
					if (LevelGenerator::RuleLevelCell* AdditionalRoom = TryGenerateSideRoom(SideRoom))
					{
						for (const DirectionType Direction : LevelGenerator::NavigationalDirections::GetDirections())
						{
							// Get Neighbor
							LevelGenerator::RuleLevelCell* SideRoomNeighborCell = StateData.GetNeighborCell(AdditionalRoom, Direction);
							const bool bNotStartAndGoal = SideRoomNeighborCell != StateData.GoalCell && SideRoomNeighborCell != StateData.StartCell;
							const bool bNotFirstSideRoom = SideRoomNeighborCell != SideRoom;
							const bool bIsInCurrentPath = std::ranges::find(StateData.GetCurrentPathway(), SideRoomNeighborCell) != StateData.GetCurrentPathway().end();

							if (bNotStartAndGoal && bNotFirstSideRoom && bIsInCurrentPath)
							{
								AdditionalRoom->CarveEntrance(SideRoomNeighborCell->GetSpatialHash());
								SideRoomNeighborCell->CarveEntrance(AdditionalRoom->GetSpatialHash());
								break;
							}
						}
					}
				}

				for (const DirectionType Direction : RandomDirections)
				{
					LevelGenerator::RuleLevelCell* SideRoomNeighborCell = StateData.GetNeighborCell(SideRoom, Direction);

					if (SideRoomNeighborCell && SideRoomNeighborCell->GetNumberOfEntrances() < 3 && SideRoomNeighborCell != StateData.GoalCell && SideRoomNeighborCell != StateData.StartCell && SideRoomNeighborCell != Cell && std::ranges::find(StateData.GetCurrentPathway(), SideRoomNeighborCell) != StateData.GetCurrentPathway().end())
					{
						//SideRoom->CarveEntrance(SideRoomNeighborCell->GetSpatialHash());
						//SideRoomNeighborCell->CarveEntrance(SideRoom->GetSpatialHash());
						break;
					}
				}

				if (StateData.CurrentInsertionIndex == 0)
				{
					StateData.CurrentInsertionIndex++;
				}
				else
				{
					StateData.CurrentInsertionIndex--;
				}

				break;
			}
		}

		if (PreviousInsertionIndex == StateData.CurrentInsertionIndex)
		{
			if (StateData.CurrentInsertionIndex == 0)
			{
				StateData.CurrentInsertionIndex++;
			}
			else
			{
				StateData.CurrentInsertionIndex--;
			}

		}

		if (StateData.GetNumberOfEmptySlots() == 0)
		{
			StateData.CurrentAction = LevelGenerator::GeneratorActionType::Done;
			StateData.CurrentInsertionIndex = PreviousInsertionIndex;
		}
	}

	void AddSideRoomsCommand::Undo()
	{
		StateData.CurrentAction = LevelGenerator::GeneratorActionType::FillEmptySlots;
		StateData.CurrentInsertionIndex = PreviousInsertionIndex;

		for (const auto& [SideRoom, InsertionIndex] : AddedSideRooms)
		{
			for (const DirectionType Direction : LevelGenerator::NavigationalDirections::GetDirections())
			{
				if ((SideRoom->GetEntranceFlag() & Direction) != DirectionType::None)
				{
					LevelGenerator::RuleLevelCell* Neighbor = StateData.GetNeighborCell(SideRoom, Direction);
					SideRoom->CollapseEntrance(Neighbor->GetSpatialHash());
					Neighbor->CollapseEntrance(SideRoom->GetSpatialHash());

					if (SideRoom->GetEntranceFlag() == DirectionType::None)
					{
						break;
					}
				}
			}

			std::erase(StateData.GetPathway(InsertionIndex), SideRoom);
			SideRoom->SetVisited(false);
		}
	}

	LevelGenerator::RuleLevelCell* AddSideRoomsCommand::TryGenerateSideRoom(LevelGenerator::RuleLevelCell* CurrentCell)
	{
		if (CurrentCell == StateData.GoalCell || CurrentCell == StateData.StartCell)
		{
			return nullptr;
		}

		const auto Directions = StateData.GetAvailableDirections(CurrentCell);

		for (DirectionType Direction : Directions)
		{
			LevelGenerator::RuleLevelCell* SideRoomCell = StateData.GetNeighborCell(CurrentCell, Direction);

			if (CurrentCell->GetNumberOfEntrances() < 3)
			{
				CurrentCell->CarveEntrance(SideRoomCell->GetSpatialHash());
				SideRoomCell->CarveEntrance(CurrentCell->GetSpatialHash());

				SideRoomCell->SetVisited(true);
				StateData.GetPathway(StateData.CurrentInsertionIndex).push_back(SideRoomCell);

				AddedSideRooms.emplace_back(SideRoomCell, StateData.CurrentInsertionIndex);
				return SideRoomCell;
			}
		}

		return nullptr;
	}


}