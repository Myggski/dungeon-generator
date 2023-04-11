#pragma once
#include <array>

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelCell.h"
#include <LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h>
#include <LevelGeneration/LevelGenerator/RuleLevel/NavigationalDirections.h>

namespace Cyclic
{
	class CyclicRule;
}

namespace Command
{
	class AddSideRoomsCommand : public Command
	{
	public:
		AddSideRoomsCommand(LevelGenerator::RuleLevelStateData& StateData, std::array<DirectionType, 4> RandomDirections, bool AddAdditionalRoom);

		void Execute() override;
		void Undo() override;
	private:
		LevelGenerator::RuleLevelCell* TryGenerateSideRoom(LevelGenerator::RuleLevelCell* CurrentCell);

	private:
		LevelGenerator::RuleLevelStateData& StateData;
		std::array<DirectionType, 4> RandomDirections;
		bool AddAdditionalRoom;
		int PreviousInsertionIndex;
		std::vector<std::tuple<LevelGenerator::RuleLevelCell*, int>> AddedSideRooms;
	};
}

