#include "RoomRepository.h"

#include <algorithm>

namespace LevelGeneration
{
	void RoomRepository::Add(const Room&& RoomToAdd)
	{
		Rooms.emplace_back(RoomToAdd);
	}

	void RoomRepository::Remove(Room RoomToRemove)
	{
		Rooms.erase(std::ranges::remove_if(Rooms, [RoomToRemove](LevelGeneration::Room const& Room)
		{
			return Room.RoomId == RoomToRemove.RoomId;
		}).begin(), Rooms.end());
	}

	std::vector<Room>& RoomRepository::GetAll() const
	{
		return Rooms;
	}
}
