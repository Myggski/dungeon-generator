#include "room_factory.h"

namespace level_generation
{
	void room_factory::generate_fragments(room& room, int number_of_fragments)
	{
		for (int index = 0; index < number_of_fragments; index++)
		{
			room.try_generate_fragment();
		}
	}

	room room_factory::get_simple_room(SDL_Rect rect)
	{
		return room { rect };
	}

	room room_factory::get_small_room(SDL_Rect rect)
	{
		room room = get_simple_room(rect);
		generate_fragments(room, 1);

		return room;
	}

	room room_factory::get_medium_room(SDL_Rect rect)
	{
		room room = get_simple_room(rect);
		generate_fragments(room, 2);

		return room;
	}

	room room_factory::get_large_room(SDL_Rect rect)
	{
		room room = get_simple_room(rect);
		generate_fragments(room, 3);

		return room;
	}

	room room_factory::get_extra_large_room(SDL_Rect rect)
	{
		room room = get_simple_room(rect);
		generate_fragments(room, 4);

		return room;
	}
}
