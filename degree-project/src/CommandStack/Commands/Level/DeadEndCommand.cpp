#include "DeadEndCommand.h"

namespace Command
{
	DeadEndCommand::DeadEndCommand(LevelGenerator::RuleLevelStateData& RuleLevelStateData)
	: RuleLevelStateData(RuleLevelStateData),
	DeadEndCell(nullptr) { }

	void DeadEndCommand::Execute()
	{
		DeadEndCell = RuleLevelStateData.CurrentCell;
		RuleLevelStateData.VisitedCellStack.pop();
		
		if (!RuleLevelStateData.VisitedCellStack.empty())
		{
			RuleLevelStateData.CurrentCell = RuleLevelStateData.VisitedCellStack.top();
			RuleLevelStateData.CurrentCell->CollapseEntrance(DeadEndCell->GetSpatialHash());
			DeadEndCell->CollapseEntrance(RuleLevelStateData.CurrentCell->GetSpatialHash());
			RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::CarvingPath;
		}
	}

	void DeadEndCommand::Undo()
	{
		RuleLevelStateData.CurrentCell->CarveEntrance(DeadEndCell->GetSpatialHash());
		DeadEndCell->CarveEntrance(RuleLevelStateData.CurrentCell->GetSpatialHash());
		RuleLevelStateData.VisitedCellStack.push(DeadEndCell);
		RuleLevelStateData.CurrentCell = DeadEndCell;
		RuleLevelStateData.CurrentAction = LevelGenerator::GeneratorActionType::ReachedDeadEnd;
	}
}
