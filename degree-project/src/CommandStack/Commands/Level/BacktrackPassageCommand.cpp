#include "BacktrackPassageCommand.h"

#include <vector>

#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"

namespace Command
{
	BacktrackPassageCommand::BacktrackPassageCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData)
		: PathReversed(false),
		RuleLevelStateData(RuleLevelStateData),
		PreviousCell(nullptr) { }

	void BacktrackPassageCommand::Execute()
	{
		PreviousCell = RuleLevelStateData.CurrentCell;

		if (!RuleLevelStateData.VisitedCellStack.empty())
		{
			RuleLevelStateData.AddCellToPathway(RuleLevelStateData.VisitedCellStack.top());
			RuleLevelStateData.VisitedCellStack.pop();
		} else
		{
			RuleLevelStateData.CurrentCell = RuleLevelStateData.StartCell;
		}

		if (!RuleLevelStateData.VisitedCellStack.empty())
		{
			RuleLevelStateData.CurrentCell = RuleLevelStateData.VisitedCellStack.top();
		} else if (RuleLevelStateData.CurrentCell == RuleLevelStateData.StartCell)
		{
			if (RuleLevelStateData.CurrentInsertionIndex > 0)
			{
				RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::AddMainRuleElements;
			}
			else
			{
				RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::CreatingPath;
			}

			RuleLevelStateData.ReversePathway(RuleLevelStateData.CurrentInsertionIndex);
			PathReversed = true;
		}
	}

	void BacktrackPassageCommand::Undo()
	{
		if (PathReversed)
		{
			RuleLevelStateData.ReversePathway(RuleLevelStateData.CurrentInsertionIndex);
		}

		if (!RuleLevelStateData.IsPathwayEmpty())
		{
			const std::vector<LevelGenerator::RuleLevelCell*>& CurrentPathway = RuleLevelStateData.GetCurrentPathway();
			LevelGenerator::RuleLevelCell* LastCellInPath = CurrentPathway[CurrentPathway.size() - 1];

			RuleLevelStateData.VisitedCellStack.push(LastCellInPath);
			RuleLevelStateData.RemoveLastCellFromPathway();
		}

		RuleLevelStateData.CurrentCell = PreviousCell;
	}
}
