#pragma once
#include "CommandStack/Commands/Command.h"

namespace LevelGenerator
{
	struct LevelStateData;
}

namespace Command
{
	class AddSideRoomsCommand : public Command
	{
	public:
		AddSideRoomsCommand(LevelGenerator::LevelStateData& StateData);

		void Execute() override;
		void Undo() override;

	private:
		LevelGenerator::LevelStateData& StateData;
	};
}

