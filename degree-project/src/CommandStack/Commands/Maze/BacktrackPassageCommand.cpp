#include "BacktrackPassageCommand.h"

#include <vector>

#include "LevelGeneration/LevelGenerator/LevelStateData.h"

namespace Command
{
	BacktrackPassageCommand::BacktrackPassageCommand(LevelGenerator::LevelStateData& StateData)
		: PathReversed(false),
		  StateData(StateData),
		  PreviousCell(nullptr)
	{ }

	void BacktrackPassageCommand::Execute()
	{
		PreviousCell = StateData.CurrentCell;

		if (!StateData.VisitedCellStack.empty())
		{
			StateData.AddCellToPathway(StateData.VisitedCellStack.top());
			StateData.VisitedCellStack.pop();
		} else
		{
			StateData.CurrentCell = StateData.StartCell;
		}

		if (!StateData.VisitedCellStack.empty())
		{
			StateData.CurrentCell = StateData.VisitedCellStack.top();
		} else if (StateData.CurrentCell == StateData.StartCell)
		{
			if (StateData.CurrentInsertionIndex > 0)
			{
				StateData.CurrentAction = LevelGenerator::GeneratorActionType::Done;
			}
			else
			{
				StateData.CurrentAction = LevelGenerator::GeneratorActionType::CreatingPath;
			}

			StateData.ReversePathway(StateData.CurrentInsertionIndex);
			PathReversed = true;
		}
	}

	void BacktrackPassageCommand::Undo()
	{
		if (PathReversed)
		{
			StateData.ReversePathway(StateData.CurrentInsertionIndex);
		}

		if (!StateData.IsPathwayEmpty())
		{
			const std::vector<LevelGenerator::LevelCell*>& CurrentPathway = StateData.GetCurrentPathway();
			LevelGenerator::LevelCell* LastCellInPath = CurrentPathway[CurrentPathway.size() - 1];

			StateData.VisitedCellStack.push(LastCellInPath);
			StateData.RemoveLastCellFromPathway();
		}

		StateData.CurrentCell = PreviousCell;
	}
}
