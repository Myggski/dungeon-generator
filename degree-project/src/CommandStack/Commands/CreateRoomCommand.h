#pragma once

#include <SDL_rect.h>

#include "Command.h"

namespace LevelGeneration
{
	class Room;
	class RoomRepository;
	enum class RoomSize;
}

namespace Command
{
	class CreateRoomCommand final : Command
	{
	public:
		~CreateRoomCommand() override = default;
		CreateRoomCommand(LevelGeneration::RoomRepository& RoomRepository, SDL_Point Position, LevelGeneration::RoomSize RoomType);

		void Execute() override;
		void Undo() override;

	private:
		SDL_Point Position;
		LevelGeneration::RoomSize RoomType;
		LevelGeneration::RoomRepository& RoomRepository;

	};
}
