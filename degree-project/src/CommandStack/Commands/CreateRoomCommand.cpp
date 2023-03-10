#include "CreateRoomCommand.h"

namespace Command
{
	CreateRoomCommand::CreateRoomCommand(LevelGeneration::RoomRepository& RoomRepository, SDL_Point Position, LevelGeneration::RoomType RoomType)
		: Position(Position),
		  RoomType(RoomType),
		  RoomRepository(RoomRepository) { }


	void CreateRoomCommand::Execute()
	{

	}

	void CreateRoomCommand::Undo()
	{
	}
}
