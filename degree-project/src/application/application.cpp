
#include "Application.h"
#include <cassert>
#include <SDL.h>

#include "LevelGeneration/RoomFactory.h"
#include "CommandStack/CommandStack.h"
#include "LevelGeneration/MazeGenerator/DirectionType.h"
#include "LevelGeneration/MazeGenerator/Maze.h"
#include "utils/RandomGenerator.h"

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

		Utils::RandomGenerator::CreateInstance();
		Command::CommandStack::CreateInstance(10000);

		Events.AddListener(SDL_QUIT, &ExitApplication);
		Events.AddListener(SDL_KEYDOWN, &OnKeyDown);
		Events.AddListener(SDL_KEYUP, &OnKeyReleased);
	}

	void Application::Run()
	{
		bIsRunning = true;
		Time.Init();

		MazeGenerator::Maze Maze{ 6, 6 };
		

		std::optional<LevelGeneration::Room> RoomXL;

		while (bIsRunning)
		{
			Renderer.ClearCanvas();

			Time.RefreshDT();
			Events.Pull();

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_SPACE))
			{
				//RoomXL = LevelGeneration::RoomFactory::CreateRoom({ 23, 12 }, LevelGeneration::RoomType::Large);
				Maze.CarvePassage();
			}

			// Update logic
			if (RoomXL.has_value())
			{
				RoomXL.value().Draw(Renderer);
			}

			auto grid = Maze.GetGrid();

			for (int x = 0; x < 6; x++)
			{
				for (int y = 0; y < 6; y++)
				{
					DirectionType flags = grid[x][y].GetEntranceFlag();
					int a = static_cast<int>(grid[x][y].GetEntranceFlag());

					SDL_Rect TextureRect{
							x * 32,
							y * 32,
							32,
							32
					};

					if (grid[x][y].GetEntranceFlag() != DirectionType::None || Maze.GetCurrent().GetPosition().x == x && Maze.GetCurrent().GetPosition().y == y)
					{
						auto* image = Maze.GetCurrent().GetPosition().x == x && Maze.GetCurrent().GetPosition().y == y ? Renderer.GetImage("resources/" + std::to_string(0) + ".png") : Renderer.GetImage("resources/" + std::to_string(static_cast<int>(flags)) + ".png");
						Renderer.DrawTexture(image, TextureRect, 0);
					}
					

				}
			}

			Renderer.SetDrawColor(0, 0, 0);
			Renderer.DrawRectangle({ 0, 0, 6, 6 });
			Renderer.DrawCanvas();
		}

		Exit();
	}

	void Application::Exit()
	{
		Events.Clear();
		Renderer.Clear();

		SDL_DestroyWindow(Window);
		Window = nullptr;

		SDL_Quit();
	}
}
