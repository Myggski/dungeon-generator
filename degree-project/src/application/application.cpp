
#include "Application.h"
#include <cassert>
#include <SDL.h>

#include "LevelGeneration/RoomFactory.h"
#include "CommandStack/CommandStack.h"
#include "LevelGeneration/MazeGenerator/DirectionType.h"
#include "LevelGeneration/MazeGenerator/Maze.h"
#include "utils/RandomGenerator.h"
#include "Application/Font.h"

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
		Font::CreateInstance(Renderer);
		
		Events.AddListener(SDL_QUIT, &ExitApplication);
		Events.AddListener(SDL_KEYDOWN, &OnKeyDown);
		Events.AddListener(SDL_KEYUP, &OnKeyReleased);

		Font::GetInstance().Load("Default", "resources/fonts/Silver.ttf", 32);
	}

	void Application::Run()
	{
		bIsRunning = true;
		Time.Init();
		const int GridSizeX = 7;
		const int GridSizeY = 5;

		MazeGenerator::Maze Maze{ GridSizeX, GridSizeY };

		std::optional<LevelGeneration::Room> RoomXL;

		while (bIsRunning)
		{
			Renderer.ClearCanvas();

			Time.RefreshDT();
			Events.Pull();

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_SPACE))
			{
				//RoomXL = LevelGeneration::RoomFactory::CreateRoom({ 23, 12 }, LevelGeneration::RoomType::Large);
				Maze.CarveCellPassage();
			}

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_LEFT))
			{
				Command::CommandStack::GetInstance().Undo();
			}

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_RIGHT))
			{
				Command::CommandStack::GetInstance().Redo();
			}

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_R))
			{
				Command::CommandStack::GetInstance().Clear();
				Utils::RandomGenerator::GetInstance().RandomizeSeed();
				Maze = { GridSizeX, GridSizeY };
			}

			// Update logic
			if (RoomXL.has_value())
			{
				RoomXL.value().Draw(Renderer);
			}

			auto grid = Maze.GetGrid();

			for (int x = 0; x < GridSizeX; x++)
			{
				for (int y = 0; y < GridSizeY; y++)
				{
					DirectionType flags = grid[x][y].GetEntranceFlag();
					int a = static_cast<int>(grid[x][y].GetEntranceFlag());

					SDL_Rect TextureRect{
							x * 32,
							y * 32,
							32,
							32
					};

					if (grid[x][y].GetEntranceFlag() != DirectionType::None)
					{
						auto* image = Renderer.GetImage("resources/" + std::to_string(static_cast<int>(flags)) + ".png");
						Renderer.DrawTexture(image, TextureRect, 0);
					}

					if (Maze.GetCurrentCell().GetPosition().x == x && Maze.GetCurrentCell().GetPosition().y == y)
					{
						auto* image = Renderer.GetImage("resources/" + std::to_string(0) + ".png");
						Renderer.DrawTexture(image, TextureRect, 0);
					}

					if (Maze.GetStartCell().GetPosition().x == x && Maze.GetStartCell().GetPosition().y == y)
					{
						auto* image = Renderer.GetImage("resources/start.png");
						Renderer.DrawTexture(image, TextureRect, 0);
					}

					if (Maze.GetGoalCell().GetPosition().x == x && Maze.GetGoalCell().GetPosition().y == y)
					{
						auto* image = Renderer.GetImage("resources/goal.png");
						Renderer.DrawTexture(image, TextureRect, 0);
					}
				}
			}

			if (!Maze.GetPathway().empty())
			{
				for (auto* maze : Maze.GetPathway())
				{
					SDL_Rect TextureRect{
						maze->GetPosition().x * 32,
						maze->GetPosition().y * 32,
						32,
						32
					};

					auto* image = Renderer.GetImage("resources/visited.png");
					Renderer.DrawTexture(image, TextureRect, 0);

					if (Maze.GetGoalCell().GetPosition().x == maze->GetPosition().x && Maze.GetGoalCell().GetPosition().y == maze->GetPosition().y)
					{
						auto* image = Renderer.GetImage("resources/goal.png");
						Renderer.DrawTexture(image, TextureRect, 0);
					}
				}
			}

			Maze.DrawDebugText();

			Renderer.SetDrawColor(0, 0, 0);
			Renderer.DrawRectangle({ 0, 0, GridSizeX, GridSizeY });
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
