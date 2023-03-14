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
			throw std::logic_error("Instance of CommandStack is already created!");
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
		for (size_t Index = Commands.size() - 1; Index > CurrentIndex; --Index)
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

	bool CommandStack::IsEmpty() const
	{
		return CurrentIndex < 0;
	}
}
