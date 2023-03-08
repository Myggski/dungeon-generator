#pragma once

#include <SDL_stdinc.h>

namespace application
{
	struct timer final {
	public:
		timer();

		void init();
		void refresh_dt();

	public:
		const float& delta_time;

	private:
		Uint64 previous_ticks;
		float dt;
	};
}
