#include "Room.h"

#include "Application/Renderer.h"
#include "Utils/RandomGenerator.h"
#include "Application/Font.h"

namespace LevelGeneration
{
	Room::Room()
		: RoomRect({}),
		Type(LevelGenerator::RoomType::None),
		RoomId(std::numeric_limits<size_t>::min()) {}

	Room::Room(SDL_FRect RoomRect)
		: RoomId(NextRoomId++),
		Type(LevelGenerator::RoomType::None),
		RoomRect(RoomRect) { }

	bool Room::operator==(const Room& Other) const
	{
		return RoomId == Other.RoomId;
	}

	void Room::Draw(Application::Renderer& Renderer, const std::tuple<int, int>& CellSize, SDL_Color Color)
	{
		for (int X = 0; X < RoomRect.w; X++)
		{
			for (int Y = 0; Y < RoomRect.h; Y++)
			{
				//Renderer.DrawTexture(FloorTexture, TextureRect, 0);
				if (X == RoomRect.w - 1 || Y == RoomRect.h - 1)
				{
					const int HalfWidth = (RoomRect.w / 2) - 1;
					const int HalfHeight = (RoomRect.h / 2) - 1;
					const bool EntranceNorth = HasEntrance(DirectionType::North) && Y == 0 && (X == HalfWidth);
					const bool EntranceEast = HasEntrance(DirectionType::East) && X == RoomRect.w - 1 && (Y == HalfHeight);
					const bool EntranceSouth = HasEntrance(DirectionType::South) && Y == RoomRect.h - 1 && (X == HalfWidth);
					const bool EntranceWest = HasEntrance(DirectionType::West) && X == 0 && (Y == HalfHeight);

					if (EntranceNorth || EntranceEast || EntranceSouth || EntranceWest)
					{
						Renderer.SetDrawColor(199, 220, 208);
					}
					else {
						Renderer.SetDrawColor(49, 54, 56);
					}
				}
				else 
				{
					Renderer.SetDrawColor(Color.r, Color.g, Color.b);
				}

				Renderer.DrawFilledRectangle({ RoomRect.x + static_cast<float>(X), RoomRect.y + static_cast<float>(Y), 1, 1 }, CellSize);

				if (X != RoomRect.w - 1 && Y != RoomRect.h - 1)
				{
					Renderer.SetDrawColor(155, 171, 178);
					Renderer.DrawRectangle({ RoomRect.x + static_cast<float>(X), RoomRect.y + static_cast<float>(Y), 1, 1 }, CellSize);
				}

				SDL_FRect TextureRect{
					 RoomRect.x  + static_cast<float>(X) - 0.5f,
					 RoomRect.y + static_cast<float>(Y) - 0.5f,
					2.f,
					2.f
				};

				if (!Elements.contains(std::to_string(X) + "." + std::to_string(Y)))
				{
					continue;
				}

				for (const auto& Element : Elements.at(std::to_string(X) + "." + std::to_string(Y)))
				{
					if (Element->GetElementType() == LevelElement::ElementType::LockAndKey)
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/key.png"), TextureRect, 0);
					}

					if (Element->GetElementType() == LevelElement::ElementType::MiniBoss)
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/monster.png"), TextureRect, 0);
					}

					if (Element->GetElementType() == LevelElement::ElementType::Reward)
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/reward.png"), TextureRect, 0);
					}

					if (Element->GetElementType() == LevelElement::ElementType::Trap)
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/trap.png"), TextureRect, 0);
					}

					if (Element->GetElementType() == LevelElement::ElementType::KillTarget)
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/kill-target.png"), TextureRect, 0);
					}

					if (Element->GetElementType() == LevelElement::ElementType::SecretDocument)
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/secret-document.png"), TextureRect, 0);
					}
				}				
			}
		}

		Application::Font::GetInstance().Draw("Default", LevelGenerator::RoomTypeToString(Type), RoomRect.x + 0.125, (RoomRect.h - 2 + RoomRect.y) + 0.125);
	}

	void Room::AdjustWidth(float GridWidthToAdjust, float MoveX)
	{
		RoomRect.x += MoveX;
		RoomRect.w += GridWidthToAdjust;
	}

	void Room::AdjustHeight(float GridHeightToAdjust, float MoveY)
	{
		RoomRect.y += MoveY;
		RoomRect.h += GridHeightToAdjust;
	}

	void Room::SetEntrance(DirectionType Entrance)
	{
		EntranceFlag = Entrance;
	}

	bool Room::HasEntrance(DirectionType EntranceDirection) const {
		return static_cast<bool>((EntranceFlag & EntranceDirection));
	}

	LevelGenerator::RoomType Room::GetType() const
	{
		return this->Type;
	}

	void Room::SetType(LevelGenerator::RoomType Type)
	{
		this->Type = Type;
	}
}