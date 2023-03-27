#pragma once

#include <vector>

#include "Room.h"

namespace LevelGeneration
{
	class RoomRepository
	{
	public:
		void Add(const Room&& Room) const;
		void Remove(Room Room) const;
		std::vector<Room>& GetAll() const;

	private:
		std::vector<Room>& Rooms;
	};
}
