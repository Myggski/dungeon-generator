#include "CommandStack.h"

#include <stdexcept>

namespace Command
{
	std::unique_ptr<CommandStack> CommandStack::StackInstance = nullptr;

	CommandStack::CommandStack(int NumberOfCommandsToReserve) : CurrentIndex(-1)
	{
		Commands.reserve(NumberOfCommandsToReserve);
	}

	void CommandStack::CreateInstance(int NumberOfCommandsToReserve)
	{
		if (StackInstance != nullptr) {
			throw std::logic_error("Instance of CommandStack has already been created!");
		}

		StackInstance = std::make_unique<CommandStack>(CommandStack(NumberOfCommandsToReserve));
	}

	CommandStack& CommandStack::GetInstance()
	{
		if (StackInstance == nullptr)
		{
			throw std::logic_error("Instance of CommandStack has not been created yet, or has been destroyed!");
		}

		return *StackInstance;
	}

	void CommandStack::ExecuteCommand(std::unique_ptr<Command> Command)
	{
		while (Commands.size() - 1 != CurrentIndex)
		{
			Commands.pop_back();
		}

		Commands.emplace_back(std::move(Command));
		Commands[++CurrentIndex]->Execute();
	}

	void CommandStack::Undo()
	{
 		if (IsEmpty())
		{
			return;
		}

		Commands[CurrentIndex--]->Undo();
	}

	void CommandStack::Redo()
	{
		if (CurrentIndex < static_cast<int>(Commands.size() - 1))
		{
			Commands[++CurrentIndex]->Execute();
		}
	}

	void CommandStack::RedoAll()
	{
		while (CurrentIndex < static_cast<int>(Commands.size() - 1))
		{
			Commands[++CurrentIndex]->Execute();
		}
	}

	void CommandStack::Clear()
	{
		Commands.clear();
		CurrentIndex = -1;
	}

	bool CommandStack::IsEmpty() const
	{
		return CurrentIndex < 0;
	}
}
