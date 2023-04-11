#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/Cyclic/CyclicRule.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"

namespace Command
{

	class AddElementCommand final : public Command
	{
	public:
		AddElementCommand(const Cyclic::CyclicRule& MainRule, LevelGenerator::RuleLevelStateData& RuleLevelStateData);

		void Execute() override;
		void Undo() override;

	private:
		const Cyclic::CyclicRule& MainRule;
		LevelGenerator::RuleLevelStateData& RuleLevelStateData;

		int FirstPathCellIndex;
		int SecondPathCellIndex;
		int FirstPathElementIndex;
		int SecondPathElementIndex;
	};
}
