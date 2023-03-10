#pragma once
#include <SDL_stdinc.h>

namespace LevelGeneration
{
	enum class RoomType : Uint8
	{
		Simple = 0,
		Small = 1,
		Medium = 2,
		Large = 3,
		ExtraLarge = 4
	};
}
