
#include "Room.h"

#include "FragmentPosition.h"
#include "RoomType.h"
#include "Application/Renderer.h"
#include "Utils/RandomGenerator.h"

namespace LevelGeneration
{
	Room::Room(SDL_Rect Rect, LevelGeneration::RoomType RoomType)
		: RoomRect(Rect),
		  FullRect({}),
		  FloorTexture(nullptr),
		  RoomType(RoomType),
		  FragmentRooms({}),
		  RoomId(NextRoomId++) { }

	void Room::GenerateFragment(FragmentPosition FragmentPosition)
	{
		if (IsFull() || RoomRect.h < MIN_ROOM_SIZE || RoomRect.w < MIN_ROOM_SIZE)
		{
			return;
		}

		const int FragmentWidth = Utils::RandomGenerator::GetInstance().GetRandom(MIN_ROOM_SIZE, RoomRect.w);
		const int FragmentHeight = Utils::RandomGenerator::GetInstance().GetRandom(MIN_ROOM_SIZE, RoomRect.h);
		const int OffsetWidth = RoomRect.w - FragmentWidth;
		const int OffsetHeight = RoomRect.h - FragmentHeight;

		// If the fragments is to the Left or Right of the Room, the offset should be up and down
		const int offset = FragmentPosition == FragmentPosition::Left || FragmentPosition == FragmentPosition::Right
			? Utils::RandomGenerator::GetInstance().GetRandom(0, OffsetHeight)
			: Utils::RandomGenerator::GetInstance().GetRandom(0, OffsetWidth);

		FragmentRooms[static_cast<int>(FragmentPosition)] = std::optional<RoomFragment>{ RoomFragment(FragmentWidth, FragmentHeight, offset) };

		SetFullRect();
	}

	void Room::RemoveFragment(FragmentPosition FragmentPosition)
	{
		FragmentRooms[static_cast<int>(FragmentPosition)] = std::nullopt;
	}


	void Room::Draw(Application::Renderer& Renderer)
	{
		if (FloorTexture == nullptr)
		{
			FloorTexture = Renderer.GetImage("resources/floor.png");
		}

		DrawRoom(Renderer, RoomRect);

		for (int FragmentIndex = 0; FragmentIndex < FragmentRooms.size(); FragmentIndex++)
		{
			if (FragmentRooms[FragmentIndex].has_value())
			{
				const SDL_Point FragmentPosition = GetFragmentPosition(static_cast<enum FragmentPosition>(FragmentIndex));
				DrawRoom(Renderer, {
					FragmentPosition.x,
					FragmentPosition.y,
					FragmentRooms[FragmentIndex].value().GetWidth(),
				FragmentRooms[FragmentIndex].value().GetHeight()
				});
			}
		}

		Renderer.SetDrawColor(255, 0, 0);
		Renderer.DrawRectangle(RoomRect);

		Renderer.SetDrawColor(94, 169, 190);
		Renderer.DrawRectangle(FullRect);
	}

	SDL_Point Room::GetFragmentPosition(FragmentPosition FragmentPosition) const
	{
		SDL_Point Position = { RoomRect.x, RoomRect.y };
		const std::optional<RoomFragment> Fragment = FragmentRooms[static_cast<int>(FragmentPosition)];

		if (!Fragment.has_value())
		{
			return Position;
		}

		switch (FragmentPosition)
		{
		case FragmentPosition::Top:
			Position.x += Fragment.value().GetOffset();
			Position.y -= Fragment.value().GetHeight();
			break;
		case FragmentPosition::Right:
			Position.x += RoomRect.w;
			Position.y += Fragment.value().GetOffset();
			break;
		case FragmentPosition::Bottom:
			Position.x += Fragment.value().GetOffset();
			Position.y += RoomRect.h;
			break;
		case FragmentPosition::Left:
			Position.x -= Fragment.value().GetWidth();
			Position.y += Fragment.value().GetOffset();
			break;
		}

		return Position;
	}


	bool Room::IsPositionOccupied(FragmentPosition FragmentPosition) const
	{
		return FragmentRooms[static_cast<int>(FragmentPosition)].has_value();
	}

	bool Room::IsFull()
	{
		auto HasValue = [&](std::optional<RoomFragment> Fragment) { return Fragment.has_value();  };

		return std::ranges::all_of(FragmentRooms, HasValue);
	}

	void Room::SetFullRect()
	{
		const int LeftFragmentWidth = FragmentRooms[static_cast<int>(FragmentPosition::Left)].has_value()
			? FragmentRooms[static_cast<int>(FragmentPosition::Left)].value().GetWidth()
			: 0;
		const int RightFragmentWidth = FragmentRooms[static_cast<int>(FragmentPosition::Right)].has_value()
			? FragmentRooms[static_cast<int>(FragmentPosition::Right)].value().GetWidth()
			: 0;
		const int TopFragmentHeight = FragmentRooms[static_cast<int>(FragmentPosition::Top)].has_value()
			? FragmentRooms[static_cast<int>(FragmentPosition::Top)].value().GetHeight()
			: 0;
		const int BottomFragmentHeight = FragmentRooms[static_cast<int>(FragmentPosition::Bottom)].has_value()
			? FragmentRooms[static_cast<int>(FragmentPosition::Bottom)].value().GetHeight()
			: 0;

		const int X = RoomRect.x - LeftFragmentWidth;
		const int Y = RoomRect.y - TopFragmentHeight;
		const int TotalW = RoomRect.w + LeftFragmentWidth + RightFragmentWidth;
		const int TotalH = RoomRect.h + TopFragmentHeight + BottomFragmentHeight;

		FullRect = {
			X,
			Y,
			TotalW,
			TotalH
		};
	}

	void Room::DrawRoom(Application::Renderer& Renderer, SDL_Rect Rect)
	{
		for (int X = 0; X < Rect.w; X++)
		{
			for (int Y = 0; Y < Rect.h; Y++)
			{
				SDL_Rect TextureRect{
					(Rect.x + X) * ROOM_TILE_SIZE,
					(Rect.y + Y) * ROOM_TILE_SIZE,
					ROOM_TILE_SIZE,
					ROOM_TILE_SIZE
				};

				Renderer.DrawTexture(FloorTexture, TextureRect, 0);

				Renderer.SetDrawColor(23, 23, 23);
				Renderer.DrawRectangle({ Rect.x + X, Rect.y + Y, 1, 1 });
			}
		}
	}

}