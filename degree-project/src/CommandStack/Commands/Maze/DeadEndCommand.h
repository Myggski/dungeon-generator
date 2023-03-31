#pragma once

#include <cstdint>

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

	class DeadEndCommand final : public Command
	{
	public:
		DeadEndCommand(LevelGenerator::LevelStateData& StateData);

		void Execute() override;
		void Undo() override;

	private:
		LevelGenerator::LevelStateData& StateData;
		LevelGenerator::LevelCell* DeadEndCell;
		DirectionType CarvedDirection;
	};
}
