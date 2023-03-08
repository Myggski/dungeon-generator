#include "room_fragment.h"

namespace level_generation
{
	room_fragment::room_fragment(int width, int height, int offset)
		: grid_width(width),
		grid_height(height),
		grid_offset(offset) { }

	int room_fragment::get_width() const
	{
		return grid_width;
	}

	int room_fragment::get_height() const
	{
		return grid_height;
	}

	int room_fragment::get_offset() const
	{
		return grid_offset;
	}
}
