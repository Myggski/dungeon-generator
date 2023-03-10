#include "AddRoomFragmentCommand.h"

#include "LevelGeneration/Room.h"
#include "utils/RandomGenerator.h"

namespace Command
{
	AddRoomFragmentCommand::AddRoomFragmentCommand(LevelGeneration::Room& Room)
		: Room(Room),
		FragmentPosition() { }

	void AddRoomFragmentCommand::Execute()
	{
		if (Room.IsFull())
		{
			return;
		}

		FragmentPosition = GetFreeFragmentPosition();
		Room.GenerateFragment(FragmentPosition);
	}

	void AddRoomFragmentCommand::Undo()
	{
		if (!Room.IsPositionOccupied(FragmentPosition))
		{
			return;
		}

		Room.RemoveFragment(FragmentPosition);
	}

	LevelGeneration::FragmentPosition AddRoomFragmentCommand::GetFreeFragmentPosition()
	{
		LevelGeneration::FragmentPosition FreeFragmentPosition = static_cast<LevelGeneration::FragmentPosition>(Utils::RandomGenerator::GetInstance().GetRandom(0, 3));

		for (int FragmentIndex = 0; FragmentIndex < 4; FragmentIndex++)
		{
			if (!Room.IsPositionOccupied(FreeFragmentPosition))
			{
				FragmentPosition = FreeFragmentPosition;
				break;
			}

			FreeFragmentPosition = static_cast<LevelGeneration::FragmentPosition>((static_cast<int>(FreeFragmentPosition) + 1) % 3);
		}

		return FreeFragmentPosition;
	}
}
