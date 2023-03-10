#pragma once
#include <SDL_stdinc.h>

namespace LevelGeneration
{
	enum class FragmentPosition : Uint8
	{
		None = 255,
		Top = 0,
		Right = 1,
		Bottom = 2,
		Left = 3
	};
}
