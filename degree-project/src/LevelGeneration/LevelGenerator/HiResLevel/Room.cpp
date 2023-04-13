#include "Room.h"

#include "Application/Renderer.h"
#include "Utils/RandomGenerator.h"

namespace LevelGeneration
{
	Room::Room()
		: RoomRect({}),
		Type(RoomType::None),
		RoomId(std::numeric_limits<size_t>::min()) {}

	Room::Room(SDL_FRect RoomRect)
		: RoomId(NextRoomId++),
		Type(RoomType::None),
		RoomRect(RoomRect) { }

	void Room::Draw(Application::Renderer& Renderer, const std::tuple<int, int>& CellSize)
	{
		for (int X = 0; X < RoomRect.w; X++)
		{
			for (int Y = 0; Y < RoomRect.h; Y++)
			{
				//Renderer.DrawTexture(FloorTexture, TextureRect, 0);
				if (X == 0 || X == RoomRect.w - 1 || Y == 0 || Y == RoomRect.h - 1)
				{
					Renderer.SetDrawColor(49, 54, 56);
				}
				else {
					Renderer.SetDrawColor(199, 220, 208);
				}

				Renderer.DrawFilledRectangle({ RoomRect.x + static_cast<float>(X), RoomRect.y + static_cast<float>(Y), 1, 1 }, CellSize);
			}
		}
	}
}