#include "RoomFragment.h"

namespace LevelGeneration
{
	RoomFragment::RoomFragment(int Width, int Height, int Offset)
		: GridWidth(Width),
		GridHeight(Height),
		GridOffset(Offset) { }

	int RoomFragment::GetWidth() const
	{
		return GridWidth;
	}

	int RoomFragment::GetHeight() const
	{
		return GridHeight;
	}

	int RoomFragment::GetOffset() const
	{
		return GridOffset;
	}
}
