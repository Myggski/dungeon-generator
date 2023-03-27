#include "Level.h"

#include <string>
#include <utility>
#include "imgui.h"

#include "Application/Camera.h"
#include "Application/Configuration.h"
#include "Application/Keyboard.h"
#include "CommandStack/CommandStack.h"

namespace LevelGeneration
{
	Level::Level(Cyclic::CyclicRule MainRule, int GridSizeX, int GridSizeY)
		: GridSizeX(GridSizeX),
		GridSizeY(GridSizeY),
		NumberOfFailedAttempts(0),
		MainRule(std::move(MainRule)),
		Maze(this->MainRule, GridSizeX, GridSizeY) { }

	void Level::Update(float DeltaTime)
	{
		CheckInput();
		GenerateLevel(DeltaTime);
	}

	void Level::Draw(Application::Renderer& Renderer)
	{
		DrawMaze(Renderer);

		Renderer.SetDrawColor(32, 32, 32);
		Renderer.DrawRectangle({ 0.f, 0.f, static_cast<float>(GridSizeX), static_cast<float>(GridSizeY) });

		DrawShortcutsWindow();
		DrawDebugTextWindow();
	}

	void Level::GenerateLevel(float DeltaTime)
	{
		if (!bGenerateLevel || bLevelDone)
		{
			return;
		}

		if (SinceLastStepSeconds > WaitStepTimeSeconds)
		{
			const MazeGenerator::MazeActionType CurrentStep = Maze.Step();

			if (CurrentStep == MazeGenerator::MazeActionType::CarvePassageFailed)
			{
				NumberOfFailedAttempts++;
				Command::CommandStack::GetInstance().Clear();
				Maze = { MainRule, GridSizeX, GridSizeY };
			}

			if (CurrentStep == MazeGenerator::MazeActionType::Done)
			{
				bLevelDone = true;
			}

			SinceLastStepSeconds = 0.f;
		}

		SinceLastStepSeconds += DeltaTime;
	}

	void Level::CheckInput()
	{
		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_SPACE))
		{
			bGenerateLevel = !bGenerateLevel;
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_LEFT))
		{
			Command::CommandStack::GetInstance().Undo();
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_RIGHT))
		{
			Command::CommandStack::GetInstance().Redo();
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_R))
		{
			Utils::RandomGenerator::GetInstance().RandomizeSeed();
			ResetMaze();
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_P))
		{
			ResetMaze();
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_PLUS))
		{
			if (GridSizeX < Configuration::MaxGridSize && GridSizeY < Configuration::MaxGridSize)
			{
				GridSizeX += 1;
				GridSizeY += 1;

				ResetMaze();
			}
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_MINUS))
		{
			if (GridSizeX > Configuration::MinGridSize && GridSizeY > Configuration::MinGridSize)
			{
				GridSizeX -= 1;
				GridSizeY -= 1;

				ResetMaze();
			}
		}
	}

	void Level::DrawMaze(Application::Renderer& Renderer)
	{
		const std::vector<std::vector<MazeGenerator::MazeCell>>& grid = Maze.StateData.MazeGrid;

		for (int x = 0; x < GridSizeX; x++)
		{
			for (int y = 0; y < GridSizeY; y++)
			{
				DirectionType flags = grid[x][y].GetEntranceFlag();

				SDL_FRect TextureRect{
						static_cast<float>(x),
						static_cast<float>(y),
						1.f,
						1.f
				};

				if (grid[x][y].GetEntranceFlag() != DirectionType::None)
				{
					auto* image = Renderer.GetImage("resources/" + std::to_string(static_cast<int>(flags)) + ".png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}

				if (Maze.StateData.CurrentCell->GetPosition().x == x && Maze.StateData.CurrentCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/" + std::to_string(0) + ".png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}

				if (Maze.StateData.StartCell->GetPosition().x == x && Maze.StateData.StartCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/start.png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}

				if (Maze.StateData.GoalCell->GetPosition().x == x && Maze.StateData.GoalCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/goal.png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}
			}
		}

		for (const auto& Pathway : Maze.StateData.Pathways)
		{

			if (Pathway.empty())
			{
				continue;
			}

			for (const MazeGenerator::MazeCell* maze : Pathway)
			{
				SDL_FRect TextureRect{
					static_cast<float>(maze->GetPosition().x),
					static_cast<float>(maze->GetPosition().y),
					1.f,
					1.f
				};

				auto* image = Renderer.GetImage("resources/visited.png");
				Renderer.DrawTexture(image, TextureRect, 0);

				if (Maze.StateData.GoalCell->GetPosition().x == maze->GetPosition().x && Maze.StateData.GoalCell->GetPosition().y == maze->GetPosition().y)
				{
					SDL_Texture* image = Renderer.GetImage("resources/goal.png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}
			}
		}
	}

	void Level::DrawShortcutsWindow()
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
		ImGui::Text("Step Duration: ");
		ImGui::SliderFloat(" ", &WaitStepTimeSeconds, 0.f, 1.f);

		ImGui::End();
	}

	void Level::DrawDebugTextWindow() const
	{
		ImGui::SetNextWindowSize({ 208, 192 });
		ImGui::SetNextWindowPos({ 16, static_cast<float>(Configuration::WindowHeight - 192 - 16) });

		ImGui::Begin("Debug");

		ImGui::Text("Seed: %d", Utils::RandomGenerator::GetInstance().GetSeed());

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Done: %s", Maze.StateData.CurrentAction == MazeGenerator::MazeActionType::Done ? "True" : "False");
		ImGui::Text("Number Of Failures: %d", NumberOfFailedAttempts);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Number Of Steps Taken: %d", Maze.PathwayCalculationData.NumberOfStepsTaken);
		ImGui::Text("Steps To Goal X: %d", Maze.PathwayCalculationData.StepsToGoalX);
		ImGui::Text("Steps To Goal Y: %d", Maze.PathwayCalculationData.StepsToGoalY);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Min Steps: %d", Maze.PathwayCalculationData.MinSteps);
		ImGui::Text("Max Steps: %d", Maze.PathwayCalculationData.MaxSteps);

		ImGui::End();
	}

	void Level::ResetMaze()
	{
		Command::CommandStack::GetInstance().Clear();
		Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());

		Maze = { MainRule, GridSizeX, GridSizeY };
		NumberOfFailedAttempts = 0;
		bLevelDone = false;
	}

}
