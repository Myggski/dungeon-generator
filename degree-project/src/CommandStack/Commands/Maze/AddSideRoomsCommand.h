#pragma once
#include <array>

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/LevelCell.h"

namespace Cyclic
{
	class CyclicRule;
}

namespace LevelGenerator
{
	struct LevelStateData;
}

namespace Command
{
	class AddSideRoomsCommand : public Command
	{
	public:
		AddSideRoomsCommand(LevelGenerator::LevelStateData& StateData, std::array<DirectionType, 4> RandomDirections, bool AddAdditionalRoom);

		void Execute() override;
		void Undo() override;
	private:
		LevelGenerator::LevelCell* TryGenerateSideRoom(LevelGenerator::LevelCell* CurrentCell);

	private:
		LevelGenerator::LevelStateData& StateData;
		std::array<DirectionType, 4> RandomDirections;
		bool AddAdditionalRoom;
		int PreviousInsertionIndex;
		std::vector<std::tuple<LevelGenerator::LevelCell*, int>> AddedSideRooms;
	};
}

