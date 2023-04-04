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

	Room RoomFactory::CreateSimpleRoom(SDL_FRect Rect, RoomSize RoomType)
	{
		return Room { Rect, RoomType };
	}

	Room RoomFactory::CreateRoom(SDL_Point Position, RoomSize RoomType)
	{
		const float Size = static_cast<float>(GetRoomSize(RoomType));
		const SDL_FRect RoomRectangle = { static_cast<float>(Position.x), static_cast<float>(Position.y), Size, Size };
		Room Room = CreateSimpleRoom(RoomRectangle, RoomType);
		GenerateFragments(Room, static_cast<int>(RoomType));

		return Room;
	}

	int RoomFactory::GetRoomSize(RoomSize RoomType)
	{
		switch (RoomType)
		{
		case RoomSize::Small:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(3, 4);

		case RoomSize::Medium:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(3, 5);

		case RoomSize::Large:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(4, 6);

		case RoomSize::ExtraLarge:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(5, 7);
		case RoomSize::Simple:
		default:
			return Utils::RandomGenerator::GetInstance().GetRandom<int>(3, 7);
		}
	}
}
