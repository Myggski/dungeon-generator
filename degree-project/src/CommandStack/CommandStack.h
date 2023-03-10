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
		static CommandStack* GetInstance();
		static void DestroyInstance();
		void ExecuteCommand(std::unique_ptr<Command> Command);
		void Undo();
		bool IsEmpty() const;

	private:
		CommandStack(int NumberOfCommandsToReserve);

	private:
		static CommandStack* StackInstance;
		std::vector<std::unique_ptr<Command>> Commands;
		int CurrentIndex;
	};
}