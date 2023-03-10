#pragma once

#include "Room.h"

namespace LevelGeneration
{
	class RoomFactory
	{
	public:
		static Room CreateSimpleRoom(SDL_Rect Rect, RoomType RoomType);
		static Room CreateRoom(SDL_Point Position, RoomType RoomType);

	private:
		static int GetRoomSize(RoomType RoomType);
		static void GenerateFragments(Room& Room, int NumberOfFragments);

		friend Room;
	};
}
