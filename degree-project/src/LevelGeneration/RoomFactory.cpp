#include "RoomFactory.h"

#include "CommandStack/CommandStack.h"
#include "CommandStack/Commands/AddRoomFragmentCommand.h"
#include "Utils/RandomGenerator.h"

namespace LevelGeneration
{
	void RoomFactory::GenerateFragments(Room& Room, int NumberOfFragments)
	{
		for (int Index = 0; Index < NumberOfFragments; Index++)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(std::make_unique<Command::AddRoomFragmentCommand>(Room));
		}
	}

	Room RoomFactory::CreateSimpleRoom(SDL_FRect Rect, RoomType RoomType)
	{
		return Room { Rect, RoomType };
	}

	Room RoomFactory::CreateRoom(SDL_Point Position, RoomType RoomType)
	{
		const float Size = static_cast<float>(GetRoomSize(RoomType));
		const SDL_FRect RoomRectangle = { static_cast<float>(Position.x), static_cast<float>(Position.y), Size, Size };
		Room Room = CreateSimpleRoom(RoomRectangle, RoomType);
		GenerateFragments(Room, static_cast<int>(RoomType));

		return Room;
	}

	int RoomFactory::GetRoomSize(RoomType RoomType)
	{
		switch (RoomType)
		{
		case RoomType::Small:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(3, 4);

		case RoomType::Medium:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(3, 5);

		case RoomType::Large:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(4, 6);

		case RoomType::ExtraLarge:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(5, 7);
		case RoomType::Simple:
		default:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(3, 7);
		}
	}
}
