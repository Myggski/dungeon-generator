#pragma once

namespace level_generation
{
	struct room_fragment
	{
	public:
		room_fragment(int width, int height, int offset);

		int get_width() const;
		int get_height() const;
		int get_offset() const;
	private:
		int grid_width;
		int grid_height;
		int grid_offset;
	};
}