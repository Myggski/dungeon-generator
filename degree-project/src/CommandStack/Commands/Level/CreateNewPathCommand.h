#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"

enum class DirectionType : uint8_t;

namespace LevelGenerator
{
	class RuleLevelGenerator;
	class RuleLevelCell;
}

namespace Command
{

	class CreateNewPathCommand final : public Command
	{
	public:
		CreateNewPathCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData);
		~CreateNewPathCommand() override;

		void Execute() override;
		void Undo() override;

	private:
		LevelGenerator::RuleLevelStateData& RuleLevelStateData;
		std::vector<LevelGenerator::RuleLevelCell*> ChangedCells;
		std::queue<DirectionType> PreviousDirections;
		LevelGenerator::RuleLevelCell* PreviousCurrentCell;
		LevelGenerator::GeneratorActionType PreviousActionType;

	};
}
