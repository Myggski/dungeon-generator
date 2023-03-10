#pragma once

namespace LevelGeneration
{
	struct RoomFragment
	{
	public:
		RoomFragment(int Width, int Height, int Offset);

		int GetWidth() const;
		int GetHeight() const;
		int GetOffset() const;
	private:
		int GridWidth;
		int GridHeight;
		int GridOffset;
	};
}