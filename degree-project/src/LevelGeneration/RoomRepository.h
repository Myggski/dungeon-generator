#pragma once
#include <vector>

#include "Room.h"

namespace LevelGeneration
{
	class RoomRepository
	{
	public:
		void Add(const Room&& Room);
		void Remove(Room Room);
		std::vector<Room>& GetAll() const;

	private:
		std::vector<Room>& Rooms;
	};
}
