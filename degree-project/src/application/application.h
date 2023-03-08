#pragma once

#include <SDL_video.h>

#include "events.h"
#include "keyboard.h"
#include "renderer.h"
#include "timer.h"

namespace application
{
	class application
	{
	public:
		explicit application();

		void init();
		void run();

	private:
		void exit();

	private:
		bool is_running{};

		SDL_Window* window{};
		events events;
		keyboard keyboard;
		timer time;
		renderer renderer;

		event_callback exit_application;
		event_callback on_key_down;
		event_callback on_key_released;
	};
}
