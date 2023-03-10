#pragma once

#include "Command.h"

namespace LevelGeneration
{
	enum class FragmentPosition : unsigned char;
	class Room;
}

namespace Command
{
	class AddRoomFragmentCommand final : public Command
	{
	public:
		AddRoomFragmentCommand(LevelGeneration::Room& Room);

		void Execute() override;
		void Undo() override;

	private:
		LevelGeneration::FragmentPosition GetFreeFragmentPosition();

	private:
		LevelGeneration::Room& Room;
		LevelGeneration::FragmentPosition FragmentPosition;
	};
}
