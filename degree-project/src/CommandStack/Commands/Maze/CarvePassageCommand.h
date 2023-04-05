#pragma once

#include "CommandStack/Commands/Command.h"
#include "LevelGeneration/LevelGenerator/LevelStateData.h"

enum class DirectionType : uint8_t;

namespace LevelGenerator
{
	class LevelGenerator;
	class LevelCell;
}

namespace Command
{

	class CarvePassageCommand final : public Command
	{
	public:
		CarvePassageCommand(LevelGenerator::LevelStateData& StateData, DirectionType MoveTowardsDirection);
		~CarvePassageCommand() override;

		void Execute() override;
		void Undo() override;

	private:
		// Resets the "bHasBeenVisited" on the dead end cells
		void ClearAllDeadEnds();

	private:
		LevelGenerator::LevelStateData& StateData;
		LevelGenerator::LevelCell* CarvedFromCell;
		LevelGenerator::LevelCell* CarvedToCell;
		DirectionType MoveTowardsDirection;
		DirectionType RemovedDirection;
		LevelGenerator::GeneratorActionType PreviousAction;
		std::vector<LevelGenerator::LevelCell*> DeadEnds;
	};
}
