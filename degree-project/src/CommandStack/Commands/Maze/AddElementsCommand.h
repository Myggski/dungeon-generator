#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/Cyclic/CyclicRule.h"
#include "LevelGeneration/LevelGenerator/LevelStateData.h"

namespace Command
{

	class AddElementCommand final : public Command
	{
	public:
		AddElementCommand(const Cyclic::CyclicRule& MainRule, LevelGenerator::LevelStateData& StateData);

		void Execute() override;
		void Undo() override;

	private:
		const Cyclic::CyclicRule& MainRule;
		LevelGenerator::LevelStateData& StateData;

		int FirstPathCellIndex;
		int SecondPathCellIndex;
		int FirstPathElementIndex;
		int SecondPathElementIndex;
	};
}
