#pragma once

#include "room.h"

namespace level_generation
{
	class room_factory
	{
	public:
		static room get_simple_room(SDL_Rect rect);
		static room get_small_room(SDL_Rect rect);
		static room get_medium_room(SDL_Rect rect);
		static room get_large_room(SDL_Rect rect);
		static room get_extra_large_room(SDL_Rect rect);

	private:
		static void generate_fragments(room& room, int number_of_fragments);

		friend room;
	};
}
