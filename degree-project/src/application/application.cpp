
#include "Application.h"
#include <cassert>
#include <SDL.h>

#include "LevelGeneration/RoomFactory.h"
#include "CommandStack/CommandStack.h"
#include "LevelGeneration/MazeGenerator/DirectionType.h"
#include "LevelGeneration/MazeGenerator/Maze.h"
#include "utils/RandomGenerator.h"
#include "Application/Font.h"
#include "Application/Configuration.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"

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
			Configuration::WindowWidth,
			Configuration::WindowHeight,
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
		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		bIsRunning = true;
		Time.Init();
		int GridSizeY = 6;
		int GridSizeX = static_cast<int>(static_cast<float>(GridSizeY) * Configuration::GridAspectRatio);

		Utils::RandomGenerator::GetInstance().RandomizeSeed();
		MazeGenerator::Maze Maze{ GridSizeX, GridSizeY };

		bool bLevelDone = false;
		bool bGenerateLevel = false;
		float WaitStepInSeconds = 0.f;
		float SinceLastStepInSeconds = 0.f;

		while (bIsRunning)
		{
			Time.RefreshDT();
			Events.Pull();

			Renderer.ClearCanvas();

			if (!bLevelDone && bGenerateLevel && SinceLastStepInSeconds > WaitStepInSeconds)
			{
				auto MazeStep = Maze.Step();

				if (MazeStep == MazeGenerator::MazeActionType::CarvePassageFailed)
				{
					Command::CommandStack::GetInstance().Clear();
					Maze = { GridSizeX, GridSizeY };
				}

				if (MazeStep == MazeGenerator::MazeActionType::Done)
				{
					bLevelDone = true;
				}

				SinceLastStepInSeconds = 0.f;
			}

			SinceLastStepInSeconds += Time.DeltaTime;

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_SPACE))
			{
				bGenerateLevel = !bGenerateLevel;
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
				bLevelDone = false;
				Utils::RandomGenerator::GetInstance().RandomizeSeed();
				Command::CommandStack::GetInstance().Clear();
				Maze = { GridSizeX, GridSizeY };
			}

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_P))
			{
				bLevelDone = false;
				Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());
				Command::CommandStack::GetInstance().Clear();
				Maze = { GridSizeX, GridSizeY };
			}

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_KP_PLUS))
			{
				if (GridSizeX < Configuration::MaxGridSize && GridSizeY < Configuration::MaxGridSize)
				{
					GridSizeX += 1;
					GridSizeY += 1;

					Command::CommandStack::GetInstance().Clear();
					Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());

					Maze = { GridSizeX, GridSizeY };
				}
			}

			if (Keyboard.IsKeyPressedOnce(SDL_SCANCODE_KP_MINUS))
			{
				if (GridSizeX > Configuration::MinGridSize && GridSizeY > Configuration::MinGridSize)
				{
					GridSizeX -= 1;
					GridSizeY -= 1;

					Command::CommandStack::GetInstance().Clear();
					Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());

					Maze = { GridSizeX, GridSizeY };
				}
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

			{
				ImGui::SetNextWindowPos({ static_cast<float>(Configuration::WindowWidth - 176 - 16), static_cast<float>(Configuration::WindowHeight - 216 - 16) });
				ImGui::SetNextWindowSize({ 176, 216 }, 0);
				ImGui::Begin("Shortcuts");

				ImGui::Text("%s - [Space]", bGenerateLevel ? "Pause" : "Play");
				ImGui::Text("Restart - [R]");
				ImGui::Text("Replay - [P]");

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Undo - [Left]");
				ImGui::Text("Redo - [Right]");

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::Text("Shrink Grid - [-]");
				ImGui::Text("Expand Grid - [+]");

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Speed: ");
				ImGui::SliderFloat(" ", &WaitStepInSeconds, 0.f, 1.f);

				ImGui::End();
			}

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

		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_Quit();
	}
}
