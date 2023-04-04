#include "DeadEndCommand.h"

namespace Command
{
	DeadEndCommand::DeadEndCommand(LevelGenerator::LevelStateData& StateData)
	: StateData(StateData),
	DeadEndCell(nullptr) { }

	void DeadEndCommand::Execute()
	{
		DeadEndCell = StateData.CurrentCell;
		StateData.VisitedCellStack.pop();
		
		if (!StateData.VisitedCellStack.empty())
		{
			StateData.CurrentCell = StateData.VisitedCellStack.top();
			StateData.CurrentCell->CollapseEntrance(DeadEndCell->GetSpatialHash());
			DeadEndCell->CollapseEntrance(StateData.CurrentCell->GetSpatialHash());
			StateData.CurrentAction = LevelGenerator::GeneratorActionType::CarvingPath;
		}
	}

	void DeadEndCommand::Undo()
	{
		StateData.CurrentCell->CarveEntrance(DeadEndCell->GetSpatialHash());
		DeadEndCell->CarveEntrance(StateData.CurrentCell->GetSpatialHash());
		StateData.VisitedCellStack.push(DeadEndCell);
		StateData.CurrentCell = DeadEndCell;
		StateData.CurrentAction = LevelGenerator::GeneratorActionType::ReachedDeadEnd;
	}
}
