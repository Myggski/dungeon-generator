#include "Timer.h"

#include <SDL_timer.h>

namespace Application
{
	Timer::Timer()
		: DeltaTime(DT),
		PreviousTicks(0),
		DT(0) { }

	void Timer::Init()
	{
		PreviousTicks = SDL_GetPerformanceCounter();
	}

	void Timer::RefreshDT()
	{
		const Uint64 Ticks = SDL_GetPerformanceCounter();
		const Uint64 NewDeltaTime = Ticks - PreviousTicks;
		PreviousTicks = Ticks;
		DT = static_cast<float>(NewDeltaTime) / static_cast<float>(SDL_GetPerformanceFrequency());
	};
}
