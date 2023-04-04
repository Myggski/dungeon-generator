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

	class CreateNewPathCommand final : public Command
	{
	public:
		CreateNewPathCommand(LevelGenerator::LevelStateData& StateData);
		~CreateNewPathCommand() override;

		void Execute() override;
		void Undo() override;

	private:
		LevelGenerator::LevelStateData& StateData;
		std::vector<LevelGenerator::LevelCell*> ChangedCells;
		std::queue<DirectionType> PreviousDirections;
		LevelGenerator::LevelCell* PreviousCurrentCell;
		LevelGenerator::GeneratorActionType PreviousActionType;

	};
}
