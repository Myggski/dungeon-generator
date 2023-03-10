#pragma once

#include <optional>
#include <array>
#include <SDL.h>

#include "FragmentPosition.h"
#include "RoomFragment.h"
#include "RoomType.h"

namespace Application
{
	class Renderer;
}

namespace LevelGeneration
{
	constexpr int MIN_ROOM_SIZE = 3;
	constexpr int ROOM_TILE_SIZE = 24;

	class Room
	{
	public:
		Room(SDL_Rect Rect, LevelGeneration::RoomType RoomType);

		void GenerateFragment(FragmentPosition FragmentPosition);
		void RemoveFragment(FragmentPosition FragmentPosition);
		void Draw(Application::Renderer& Renderer);
		bool IsPositionOccupied(FragmentPosition FragmentPosition) const;
		bool IsFull();
		size_t GetId();

	private:
		SDL_Point GetFragmentPosition(FragmentPosition FragmentPosition) const;
		void SetFullRect();
		void DrawRoom(Application::Renderer& Renderer, SDL_Rect Rect);

	private:
		SDL_Rect RoomRect; // for the Room
		SDL_Rect FullRect; // for the Room and fragments
		SDL_Texture* FloorTexture;
		LevelGeneration::RoomType RoomType;
		std::array<std::optional<RoomFragment>, 4> FragmentRooms;

		size_t RoomId;
		inline static size_t NextRoomId{ 0 };

		friend class RoomRepository;
	};
}
