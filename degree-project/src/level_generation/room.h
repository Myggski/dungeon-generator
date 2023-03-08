#pragma once

#include <optional>
#include <array>
#include <SDL.h>
#include "room_fragment.h"

namespace application
{
	class renderer;
}

namespace level_generation
{
	enum class fragment_position
	{
		top = 0,
		right = 1,
		bottom = 2,
		left = 3
	};

	constexpr int MIN_ROOM_SIZE = 2;

	class room
	{
	public:
		room(SDL_Rect rect);

		void try_generate_fragment();
		void draw(application::renderer& renderer);

	private:
		SDL_Point get_fragment_position(fragment_position fragment_position);
		bool is_position_occupied(fragment_position fragment_position) const;
		bool is_full();
		void set_full_rect();
		void draw_room(application::renderer& renderer, SDL_Rect rect);

	private:
		SDL_Rect room_rect; // for the room only
		SDL_Rect full_rect; // for the room and fragments
		SDL_Texture* floor_texture;
		std::array<std::optional<room_fragment>, 4> fragment_rooms;

		friend room_fragment;
	};
}
