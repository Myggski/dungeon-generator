#include "Level.h"

#include <string>
#include <utility>
#include "imgui.h"

#include "Application/Camera.h"
#include "Application/Configuration.h"
#include "Application/Keyboard.h"
#include "CommandStack/CommandStack.h"
#include "Cyclic/CyclicRuleRepository.h"

namespace LevelGeneration
{
	Level::Level(Cyclic::CyclicRuleRepository& RuleRepository, int GridSizeX, int GridSizeY)
		: GridSizeX(GridSizeX),
		GridSizeY(GridSizeY),
		NumberOfFailedAttempts(0),
		RuleRepository(RuleRepository),
		Maze(RuleRepository.GetRandomRule(), GridSizeX, GridSizeY)
	{
		Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());
		Maze.InitializeMaze();
	}

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
		DrawRulesInformationWindow();
	}

	void Level::GenerateLevel(float DeltaTime)
	{
		if (!bGenerateLevel || bLevelDone)
		{
			return;
		}

		if (SinceLastStepSeconds > WaitStepTimeSeconds)
		{
			const LevelGenerator::GeneratorActionType CurrentStep = Maze.Step();

			if (CurrentStep == LevelGenerator::GeneratorActionType::Failed)
			{
				NumberOfFailedAttempts++;

				Command::CommandStack::GetInstance().Clear();
				Maze = { Maze.MainRule, GridSizeX, GridSizeY };
			}

			if (CurrentStep == LevelGenerator::GeneratorActionType::Done)
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
			ResetMaze(RuleRepository.GetRandomRule());
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_P))
		{
			ResetMaze(Maze.MainRule);
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_PLUS))
		{
			if (GridSizeX < Configuration::MaxGridSize && GridSizeY < Configuration::MaxGridSize)
			{
				GridSizeX += 1;
				GridSizeY += 1;

				ResetMaze(Maze.MainRule);
			}
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_MINUS))
		{
			if (GridSizeX > Configuration::MinGridSize && GridSizeY > Configuration::MinGridSize)
			{
				GridSizeX -= 1;
				GridSizeY -= 1;

				ResetMaze(Maze.MainRule);
			}
		}
	}

	void Level::DrawMaze(Application::Renderer& Renderer) const
	{
		const std::vector<std::vector<LevelGenerator::LevelCell>>& grid = Maze.StateData.LevelGrid;

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

				if (!grid[x][y].GetElements().empty())
				{
					for (const auto& Element : grid[x][y].GetElements())
					{
						if (Element->GetElementType() == LevelElement::ElementType::LockAndKey)
						{
							SDL_Texture* image = Renderer.GetImage("resources/key.png");
							Renderer.DrawTexture(image, TextureRect, 0);
						}

						if (Element->GetElementType() == LevelElement::ElementType::MiniBoss)
						{
							SDL_Texture* image = Renderer.GetImage("resources/monster.png");
							Renderer.DrawTexture(image, TextureRect, 0);
						}

						if (Element->GetElementType() == LevelElement::ElementType::Reward)
						{
							SDL_Texture* image = Renderer.GetImage("resources/reward.png");
							Renderer.DrawTexture(image, TextureRect, 0);
						}

						if (Element->GetElementType() == LevelElement::ElementType::Trap)
						{
							SDL_Texture* image = Renderer.GetImage("resources/trap.png");
							Renderer.DrawTexture(image, TextureRect, 0);
						}
					}
				}
			}
		}

		for (const auto& Pathway : Maze.StateData.InsertionPathways)
		{
			if (Pathway.empty())
			{
				continue;
			}

			int Index = 0;

			for (const LevelGenerator::LevelCell* maze : Pathway)
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

				Index++;
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

	void Level::DrawDebugTextWindow()
	{
		ImGui::SetNextWindowSize({ 208, 192 });
		ImGui::SetNextWindowPos({ 232 + 32, static_cast<float>(Configuration::WindowHeight - 192 - 16) });

		ImGui::Begin("Debug");

		ImGui::Text("Seed: %u", Utils::RandomGenerator::GetInstance().GetSeed());

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Done: %s", bLevelDone ? "True" : "False");
		ImGui::Text("Number Of Failures: %d", NumberOfFailedAttempts);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Number Of Steps Taken: %d", Maze.PathwayCalculationData.NumberOfStepsTaken);
		ImGui::Text("Steps To Goal X: %d", Maze.PathwayCalculationData.StepsToGoalX);
		ImGui::Text("Steps To Goal Y: %d", Maze.PathwayCalculationData.StepsToGoalY);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Insertion One Steps: %llu", Maze.StateData.GetPathway(0).size());
		ImGui::Text("Insertion Two Steps: %llu", Maze.StateData.GetPathway(1).size());

		ImGui::End();
	}

	void Level::DrawRulesInformationWindow()
	{
		ImGui::SetNextWindowPos({ 16, static_cast<float>(Configuration::WindowHeight - 234 - 16) });
		ImGui::SetNextWindowSize({ 232, 234 }, 0);
		ImGui::Begin("Main Rule");

		ImGui::Text("Name:");
		ImGui::Text("%s", Maze.MainRule.GetName().c_str());
		ImGui::Text("Goal: %s", Maze.MainRule.GetGoalTypeToString().c_str());

		ImGui::Spacing();
		ImGui::Spacing();

		const auto [MinStepsOne, MaxStepsOne] = Maze.CalculateMinMaxSteps(Maze.MainRule.GetArcType(0));
		ImGui::Text("Insertion One");
		ImGui::Text("Element: %s", Maze.MainRule.GetElement(0).GetElementName().c_str());
		ImGui::Text("Path Length: %s", Maze.MainRule.GetArcType(0) == Cyclic::ArcType::Short ? "Short" : "Long");
		ImGui::Text("Steps: %d - %d", MinStepsOne, MaxStepsOne);

		ImGui::Spacing();
		ImGui::Spacing();

		const auto [MinStepsTwo, MaxStepsTwo] = Maze.CalculateMinMaxSteps(Maze.MainRule.GetArcType(1));
		ImGui::Text("Insertion Two");
		ImGui::Text("Element: %s", Maze.MainRule.GetElement(1).GetElementName().c_str());
		ImGui::Text("Path Length: %s", Maze.MainRule.GetArcType(1) == Cyclic::ArcType::Short ? "Short" : "Long");
		ImGui::Text("Steps: %d - %d", MinStepsTwo, MaxStepsTwo);

		ImGui::End();
	}

	void Level::ResetMaze(Cyclic::CyclicRule& MainRule)
	{
		Command::CommandStack::GetInstance().Clear();
		Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());

		Maze = { MainRule, GridSizeX, GridSizeY };
		NumberOfFailedAttempts = 0;
		bLevelDone = false;
	}
}
