#include "CreateRoomCommand.h"

namespace Command
{
	CreateRoomCommand::CreateRoomCommand(LevelGeneration::RoomRepository& RoomRepository, SDL_Point Position, LevelGeneration::RoomSize RoomType)
		: Position(Position),
		  RoomSize(RoomType),
		  RoomRepository(RoomRepository) { }


	void CreateRoomCommand::Execute()
	{

	}

	void CreateRoomCommand::Undo()
	{
	}
}
