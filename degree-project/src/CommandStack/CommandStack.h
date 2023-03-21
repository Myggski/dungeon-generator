#pragma once

#include <memory>
#include <vector>
#include "Commands/Command.h"

namespace Command
{
	class CommandStack
	{
	public:
		static void CreateInstance(int NumberOfCommandsToReserve);
		static CommandStack& GetInstance();
		void ExecuteCommand(std::unique_ptr<Command> Command);
		void Undo();
		void Redo();
		void Clear();
		bool IsEmpty() const;

	private:
		CommandStack(int NumberOfCommandsToReserve);

	private:
		static std::unique_ptr<CommandStack> StackInstance;
		std::vector<std::unique_ptr<Command>> Commands;
		int CurrentIndex;
	};
}