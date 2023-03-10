#pragma once

#include <SDL_stdinc.h>

namespace Application
{
	struct Timer final {
	public:
		Timer();

		void Init();
		void RefreshDT();

	public:
		const float& DeltaTime;

	private:
		Uint64 PreviousTicks;
		float DT;
	};
}
