#include "room_factory.h"

#include "utils/random_generator.h"

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

	room room_factory::get_small_room(SDL_Point position)
	{
		const int size = utils::random_generator::instance().get_random<int>(2, 4);
		room room = get_simple_room({ position.x, position.y, size, size });

		generate_fragments(room, 1);

		return room;
	}

	room room_factory::get_medium_room(SDL_Point position)
	{
		const int size = utils::random_generator::instance().get_random<int>(3, 5);
		room room = get_simple_room({ position.x, position.y, size, size });

		generate_fragments(room, 2);

		return room;
	}

	room room_factory::get_large_room(SDL_Point position)
	{
		const int size = utils::random_generator::instance().get_random<int>(4, 6);
		room room = get_simple_room({ position.x, position.y, size, size });

		generate_fragments(room, 3);

		return room;
	}

	room room_factory::get_extra_large_room(SDL_Point position)
	{
		const int size = utils::random_generator::instance().get_random<int>(5, 7);
		room room = get_simple_room({ position.x, position.y, size, size });

		generate_fragments(room, 4);

		return room;
	}
}
