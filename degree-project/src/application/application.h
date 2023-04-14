#pragma once

#include <SDL_video.h>
#include "Events.h"
#include "Renderer.h"
#include "Timer.h"

namespace Application
{
	class Application final
	{
	public:
		explicit Application();

		void Init();
		void Run();

	private:
		void Exit();

	private:
		bool bIsRunning{};

		SDL_Window* Window{};
		Events Events;
		Timer Time;
		Renderer Renderer;

		EventCallback ExitApplication;
		EventCallback OnKeyDown;
		EventCallback OnKeyReleased;
		EventCallback OnMouseDown;
		EventCallback OnMouseUp;
		EventCallback OnMouseScroll;
	};
}
