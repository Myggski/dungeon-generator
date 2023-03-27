#include "BacktrackPassageCommand.h"

#include <vector>

#include "LevelGeneration/MazeGenerator/MazeStateData.h"

namespace Command
{
	BacktrackPassageCommand::BacktrackPassageCommand(MazeGenerator::MazeStateData& StateData)
		: StateData(StateData),
		PreviousCell(nullptr) { }

	void BacktrackPassageCommand::Execute()
	{
		PreviousCell = StateData.CurrentCell;
		StateData.AddCellToPathway(StateData.VisitedCellStack.top());
		StateData.VisitedCellStack.pop();

		if (!StateData.VisitedCellStack.empty())
		{
			StateData.CurrentCell = StateData.VisitedCellStack.top();
		} else if (StateData.CurrentCell == StateData.StartCell)
		{
			StateData.CurrentAction = StateData.CurrentPathwayIndex > 0
				? MazeGenerator::MazeActionType::Done
				: MazeGenerator::MazeActionType::CreateNewPath;
		}
	}

	void BacktrackPassageCommand::Undo()
	{
		if (!StateData.IsPathwayEmpty())
		{
			const std::vector<MazeGenerator::MazeCell*>& CurrentPathway = StateData.GetCurrentPathway();
			MazeGenerator::MazeCell* LastCellInPath = CurrentPathway[CurrentPathway.size() - 1];

			StateData.VisitedCellStack.push(LastCellInPath);
			StateData.RemoveLastCellFromPathway();
		}
		
		StateData.CurrentCell = PreviousCell;
	}
}
