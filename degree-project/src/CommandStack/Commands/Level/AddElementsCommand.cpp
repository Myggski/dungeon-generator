#include "AddElementsCommand.h"

namespace Command
{
	AddElementCommand::AddElementCommand(const Cyclic::CyclicRule& MainRule, LevelGenerator::RuleLevelStateData& RuleLevelStateData)
	: MainRule(MainRule),
	RuleLevelStateData(RuleLevelStateData),
	FirstPathCellIndex(std::numeric_limits<int>::min()),
	SecondPathCellIndex(std::numeric_limits<int>::min()),
	FirstPathElementIndex(std::numeric_limits<int>::min()),
	SecondPathElementIndex(std::numeric_limits<int>::min()) { }

	void AddElementCommand::Execute()
	{
		const std::vector<LevelGenerator::RuleLevelCell*>& FirstPathway = RuleLevelStateData.GetPathway(0);
		const std::vector<LevelGenerator::RuleLevelCell*>& SecondPathway = RuleLevelStateData.GetPathway(1);
		FirstPathCellIndex = static_cast<int>(RuleLevelStateData.GetPathway(0).size() - 1) / 2;
		SecondPathCellIndex = static_cast<int>(RuleLevelStateData.GetPathway(1).size() - 1) / 2;

		FirstPathway[FirstPathCellIndex]->AddElement(MainRule.GetElement(0));
		FirstPathElementIndex = static_cast<int>(FirstPathway[FirstPathCellIndex]->GetElements().size() - 1);

		SecondPathway[SecondPathCellIndex]->AddElement(MainRule.GetElement(1));
		SecondPathElementIndex = static_cast<int>(SecondPathway[SecondPathCellIndex]->GetElements().size() - 1);

		RuleLevelStateData.GoalCell->AddElement(MainRule.GetGoalElement());

		RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::FillEmptySlots;
	}

	void AddElementCommand::Undo()
	{
		if (FirstPathElementIndex > -1 && FirstPathCellIndex > -1 )
		{
			const std::vector<LevelGenerator::RuleLevelCell*>& FirstPathway = RuleLevelStateData.GetPathway(0);
			FirstPathway[FirstPathCellIndex]->RemoveElement(FirstPathElementIndex);
			
		}

		if (SecondPathElementIndex > -1 && SecondPathCellIndex > -1)
		{
			const std::vector<LevelGenerator::RuleLevelCell*>& SecondPathway = RuleLevelStateData.GetPathway(1);
			SecondPathway[SecondPathCellIndex]->RemoveElement(SecondPathElementIndex);
		}

		RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::AddMainRuleElements;
	}
}