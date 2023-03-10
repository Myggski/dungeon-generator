
#include "Application.h"
#include <cassert>
#include <SDL.h>

#include "LevelGeneration/RoomFactory.h"
#include "CommandStack/CommandStack.h"

namespace Application
{
	Application::Application()
		: Events({}),
		Keyboard({}),
		Time({}),
		Renderer({}),
		ExitApplication([&](SDL_Event)
		{
			bIsRunning = false;
		}),
		OnKeyDown([&](SDL_Event Event)
		{
			Keyboard.OnKeyPressed(Event);
		}),
		OnKeyReleased([&](SDL_Event Event)
		{
			Keyboard.OnKeyReleased(Event);
		}) { }

	void Application::Init()
	{
		assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

		Window = SDL_CreateWindow(
			"Degree Project",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1280,
			720,
			0
		);

		SDL_RaiseWindow(Window);
		Renderer.Init(Window);
		Command::CommandStack::CreateInstance(10000);

		Events.AddListener(SDL_QUIT, &ExitApplication);
		Events.AddListener(SDL_KEYDOWN, &OnKeyDown);
		Events.AddListener(SDL_KEYUP, &OnKeyReleased);
	}

	void Application::Run()
	{
		bIsRunning = true;
		Time.Init();

		std::optional<LevelGeneration::Room> RoomXL;

		while (bIsRunning)
		{
			Renderer.ClearCanvas();

			Time.RefreshDT();
			Events.Pull();

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_SPACE))
			{
				RoomXL = LevelGeneration::RoomFactory::CreateRoom({ 23, 12 }, LevelGeneration::RoomType::Large);
			}

			// Update logic
			if (RoomXL.has_value())
			{
				RoomXL.value().Draw(Renderer);
			}

			Renderer.DrawCanvas();
		}

		Exit();
	}

	void Application::Exit()
	{
		Events.Clear();
		Renderer.Clear();
		Command::CommandStack::DestroyInstance();

		SDL_DestroyWindow(Window);
		Window = nullptr;

		SDL_Quit();
	}
}
