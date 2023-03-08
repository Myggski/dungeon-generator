
#include "application.h"
#include <cassert>
#include <SDL.h>

#include "level_generation/room_factory.h"

namespace application
{
	application::application()
		: events({}),
		keyboard({}),
		time({}),
		renderer({}),
		exit_application([&](SDL_Event)
		{
			is_running = false;
		}),
		on_key_down([&](SDL_Event event)
		{
			keyboard.on_key_pressed(event);
		}),
		on_key_released([&](SDL_Event event)
		{
			keyboard.on_key_released(event);
		}) { }

	void application::init()
	{
		assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

		window = SDL_CreateWindow(
			"Degree Project",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1280,
			720,
			0
		);

		SDL_RaiseWindow(window);
		renderer.init(window);

		events.add_listener(SDL_QUIT, &exit_application);
		events.add_listener(SDL_KEYDOWN, &on_key_down);
		events.add_listener(SDL_KEYUP, &on_key_released);
	}

	void application::run()
	{
		is_running = true;
		time.init();

		std::optional<level_generation::room> room_xl;

		while (is_running)
		{
			renderer.clear_canvas();

			time.refresh_dt();
			events.pull();

			if (keyboard.is_key_pressed_once(SDL_SCANCODE_SPACE))
			{
				// TODO: Some sort of fragment bug when it creates small rooms
				room_xl = level_generation::room_factory::get_small_room({ 17, 8 });
			}

			// Update logic
			if (room_xl.has_value())
			{
				room_xl.value().draw(renderer);
			}

			renderer.draw_canvas();
		}

		exit();
	}

	void application::exit()
	{
		events.clear();
		renderer.clear();

		SDL_DestroyWindow(window);
		window = nullptr;

		SDL_Quit();
	}
}
