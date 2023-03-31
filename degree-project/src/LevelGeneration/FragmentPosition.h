#pragma once

namespace LevelGeneration
{
	enum class FragmentPosition : uint8_t
	{
		None = 0,
		Top = 1 << 1,
		Right = 1 << 2,
		Bottom = 1 << 3,
		Left = 1 << 4
	};
}
