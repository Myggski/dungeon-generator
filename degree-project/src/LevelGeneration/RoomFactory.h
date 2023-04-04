#pragma once

#include "Room.h"

namespace LevelGeneration
{
	class RoomFactory
	{
	public:
		static Room CreateSimpleRoom(SDL_FRect Rect, RoomSize RoomType);
		static Room CreateRoom(SDL_Point Position, RoomSize RoomType);

	private:
		static int GetRoomSize(RoomSize RoomType);
		static void GenerateFragments(Room& Room, int NumberOfFragments);

		friend Room;
	};
}
