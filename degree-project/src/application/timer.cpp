#include "timer.h"

#include <SDL_timer.h>

namespace application
{
	timer::timer()
		: delta_time(dt),
		previous_ticks(0),
		dt(0) { }

	void timer::init()
	{
		previous_ticks = SDL_GetPerformanceCounter();
	}

	void timer::refresh_dt()
	{
		const Uint64 ticks = SDL_GetPerformanceCounter();
		const Uint64 delta_ticks = ticks - previous_ticks;
		previous_ticks = ticks;
		dt = static_cast<float>(delta_ticks) / static_cast<float>(SDL_GetPerformanceFrequency());
	};
}
