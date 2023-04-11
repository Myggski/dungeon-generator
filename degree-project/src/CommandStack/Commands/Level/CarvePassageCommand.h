#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"

enum class DirectionType : uint8_t;

namespace LevelGenerator
{
	class LevelGenerator;
	class RuleLevelCell;
}

namespace Command
{

	class CarvePassageCommand final : public Command
	{
	public:
		CarvePassageCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData, DirectionType MoveTowardsDirection);
		~CarvePassageCommand() override;

		void Execute() override;
		void Undo() override;

	private:
		// Resets the "bHasBeenVisited" on the dead end cells
		void ClearAllDeadEnds();

	private:
		LevelGenerator::RuleLevelStateData& RuleLevelStateData;
		LevelGenerator::RuleLevelCell* CarvedFromCell;
		LevelGenerator::RuleLevelCell* CarvedToCell;
		DirectionType MoveTowardsDirection;
		DirectionType RemovedDirection;
		LevelGenerator::GeneratorActionType PreviousAction;
		std::vector<LevelGenerator::RuleLevelCell*> DeadEnds;
	};
}
