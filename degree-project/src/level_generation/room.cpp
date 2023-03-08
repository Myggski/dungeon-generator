
#include "room.h"

#include "application/renderer.h"
#include "utils/random_generator.h"

namespace level_generation
{
	room::room(SDL_Rect rect)
		: room_rect(rect),
		fragment_rooms({}) { }

	void room::try_generate_fragment()
	{
		if (is_full() || room_rect.h < MIN_ROOM_SIZE || room_rect.w < MIN_ROOM_SIZE)
		{
			return;
		}

		const int fragment_width = utils::random_generator::instance().get_random(MIN_ROOM_SIZE, room_rect.w);
		const int fragment_height = utils::random_generator::instance().get_random(MIN_ROOM_SIZE, room_rect.h);
		const int offset_width = room_rect.w - fragment_width;
		const int offset_height = room_rect.h - fragment_height;
		fragment_position fragment_position = static_cast<enum fragment_position>(utils::random_generator::instance().get_random(0, static_cast<int>(fragment_rooms.size() - 1)));

		for (int index = 0; index < fragment_rooms.size(); index++)
		{
			if (!is_position_occupied(fragment_position))
			{
				// If the fragments is to the left or right of the room, the offset should be up and down
				const int offset = fragment_position == fragment_position::left || fragment_position == fragment_position::right
					? utils::random_generator::instance().get_random(0, offset_height)
					: utils::random_generator::instance().get_random(0, offset_width);

				fragment_rooms[static_cast<int>(fragment_position)] = std::optional<room_fragment>{ room_fragment(fragment_width, fragment_height, offset) };
				break;
			}

			fragment_position = static_cast<enum fragment_position>((static_cast<int>(fragment_position) + 1) % (fragment_rooms.size() - 1));
		}

		set_full_rect();
	}

	void room::draw(application::renderer& renderer)
	{
		if (floor_texture == nullptr)
		{
			floor_texture = renderer.get_image("resources/floor.png");
		}

		draw_room(renderer, room_rect);

		for (int fragment_index = 0; fragment_index < fragment_rooms.size(); fragment_index++)
		{
			if (fragment_rooms[fragment_index].has_value())
			{
				SDL_Point fragment_position = get_fragment_position(static_cast<enum fragment_position>(fragment_index));
				draw_room(renderer, {
					fragment_position.x,
					fragment_position.y,
					fragment_rooms[fragment_index].value().get_width(),
				fragment_rooms[fragment_index].value().get_height()
				});
			}
		}

		renderer.set_draw_color(255, 0, 0);
		renderer.draw_rectangle(room_rect);

		renderer.set_draw_color(94, 169, 190);
		renderer.draw_rectangle(full_rect);
	}

	SDL_Point room::get_fragment_position(fragment_position fragment_position)
	{
		SDL_Point position = { room_rect.x, room_rect.y };
		const std::optional<room_fragment> fragment = fragment_rooms[static_cast<int>(fragment_position)];

		if (!fragment.has_value())
		{
			return position;
		}

		switch (fragment_position)
		{
		case fragment_position::top:
			position.x += fragment.value().get_offset();
			position.y -= fragment.value().get_height();
			break;
		case fragment_position::right:
			position.x += room_rect.w;
			position.y += fragment.value().get_offset();
			break;
		case fragment_position::bottom:
			position.x += fragment.value().get_offset();
			position.y += room_rect.h;
			break;
		case fragment_position::left:
			position.x -= fragment.value().get_width();
			position.y += fragment.value().get_offset();
			break;
		}

		return position;
	}


	bool room::is_position_occupied(fragment_position fragment_position) const
	{
		return fragment_rooms[static_cast<int>(fragment_position)].has_value();
	}

	bool room::is_full()
	{
		auto has_value = [&](std::optional<room_fragment> fragment) { return fragment.has_value();  };

		return std::ranges::all_of(fragment_rooms, has_value);
	}

	void room::set_full_rect()
	{
		const int left_fragment_width = fragment_rooms[static_cast<int>(fragment_position::left)].has_value()
			? fragment_rooms[static_cast<int>(fragment_position::left)].value().get_width()
			: 0;
		const int right_fragment_width = fragment_rooms[static_cast<int>(fragment_position::right)].has_value()
			? fragment_rooms[static_cast<int>(fragment_position::right)].value().get_width()
			: 0;
		const int top_fragment_height = fragment_rooms[static_cast<int>(fragment_position::top)].has_value()
			? fragment_rooms[static_cast<int>(fragment_position::top)].value().get_height()
			: 0;
		const int bottom_fragment_height = fragment_rooms[static_cast<int>(fragment_position::bottom)].has_value()
			? fragment_rooms[static_cast<int>(fragment_position::bottom)].value().get_height()
			: 0;

		const int x = room_rect.x - left_fragment_width;
		const int y = room_rect.y - top_fragment_height;
		const int total_w = room_rect.w + left_fragment_width + right_fragment_width;
		const int total_h = room_rect.h + top_fragment_height + bottom_fragment_height;

		full_rect = {
			x,
			y,
			total_w,
			total_h
		};
	}

	void room::draw_room(application::renderer& renderer, SDL_Rect rect)
	{
		for (int x = 0; x < rect.w; x++)
		{
			for (int y = 0; y < rect.h; y++)
			{
				SDL_Rect texture_rect{
					(rect.x + x) * ROOM_TILE_SIZE,
					(rect.y + y) * ROOM_TILE_SIZE,
					ROOM_TILE_SIZE,
					ROOM_TILE_SIZE
				};

				renderer.draw_texture(floor_texture, texture_rect, 0);

				renderer.set_draw_color(23, 23, 23);
				renderer.draw_rectangle({ rect.x + x, rect.y + y, 1, 1 });
			}
		}
	}

}