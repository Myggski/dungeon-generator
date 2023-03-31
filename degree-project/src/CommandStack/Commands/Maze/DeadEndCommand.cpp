#include "DeadEndCommand.h"

#include "LevelGeneration/LevelGenerator/NavigationalDirections.h"

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

			// TODO: Collapse DeadEndCell and CurrentCell cleaner
			const int DirectionX = DeadEndCell->GetPosition().x - StateData.CurrentCell->GetPosition().x;
			const int DirectionY = DeadEndCell->GetPosition().y - StateData.CurrentCell->GetPosition().y;

			if (DirectionX > 0)
			{
				CarvedDirection = DirectionType::East;
			} else if (DirectionX < 0)
			{
				CarvedDirection = DirectionType::West;
			}

			if (DirectionY > 0)
			{
				CarvedDirection = DirectionType::South;
			} else if (DirectionY < 0)
			{
				CarvedDirection = DirectionType::North;
			}

			StateData.CurrentCell->CollapseEntrance(CarvedDirection);
			DeadEndCell->CollapseEntrance(LevelGenerator::OppositeDirection.at(CarvedDirection));
		}
	}

	void DeadEndCommand::Undo()
	{
		StateData.CurrentCell->CarveEntrance(CarvedDirection);
		DeadEndCell->CarveEntrance(LevelGenerator::OppositeDirection.at(CarvedDirection));
		StateData.VisitedCellStack.push(DeadEndCell);
		StateData.CurrentCell = DeadEndCell;
	}
}
