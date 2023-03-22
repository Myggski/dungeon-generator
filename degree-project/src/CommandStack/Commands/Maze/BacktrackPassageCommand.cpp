#include "BacktrackPassageCommand.h"

#include <utility>
#include <vector>

namespace Command
{
	BacktrackPassageCommand::BacktrackPassageCommand(MazeGenerator::MazeCell* CurrentCell, std::stack<MazeGenerator::MazeCell*>& VisitedStack, std::vector<MazeGenerator::MazeCell*>& Pathway, bool bGoalHasBeenReached, std::function<void(MazeGenerator::MazeCell*)> SetCurrentCellCallback)
		: bGoalHasBeenReached(bGoalHasBeenReached),
		CurrentCell(CurrentCell),
		PreviousCell(nullptr),
		VisitedStack(VisitedStack),
		Pathway(Pathway),
		SetCurrentCellCallback(std::move(SetCurrentCellCallback))
	{ }

	void BacktrackPassageCommand::Execute()
	{
		// If goal has been reached, save pathway on the way back
 		if (bGoalHasBeenReached)
		{
			Pathway.emplace_back(VisitedStack.top());
		}

		// Go back to the visited stack
		VisitedStack.pop();

		if (!VisitedStack.empty())
		{
			SetCurrentCellCallback(VisitedStack.top());
		}

		PreviousCell = CurrentCell;
	}

	void BacktrackPassageCommand::Undo()
	{
		if (!Pathway.empty())
		{
			VisitedStack.push(Pathway[Pathway.size() - 1]);
			Pathway.erase(Pathway.end() - 1);
		}
		
		SetCurrentCellCallback(PreviousCell);
	}
}
