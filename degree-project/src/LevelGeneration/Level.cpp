#include "Level.h"

#include <string>
#include <utility>
#include <memory>
#include "imgui.h"

#include "Application/Camera.h"
#include "Application/Configuration.h"
#include "Application/Keyboard.h"
#include "CommandStack/CommandStack.h"
#include "Cyclic/CyclicRuleRepository.h"
#include "LevelGenerator/LowResLevel/LowResCell.h"
#include "CommandStack/Commands/Level/CreateLowResLevelCommand.h"
#include <Utils/RandomGenerator.h>

namespace LevelGeneration
{
	Level::Level(Cyclic::CyclicRuleRepository& RuleRepository, int GridSizeX, int GridSizeY)
		: GridSizeX(GridSizeX),
		GridSizeY(GridSizeY),
		RuleLevelStateData(GridSizeX, GridSizeY),
		NumberOfFailedAttempts(0),
		RuleRepository(RuleRepository),
		LevelGenerator(RuleLevelStateData, RuleRepository.GetRandomRule()),
		ProcessState(LevelProcessState::RuleGridLevel)
	{
		Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());
		LevelGenerator.InitializeMaze();
	}

	void Level::Update(float DeltaTime)
	{
		CheckInput();

		if (ProcessState == LevelProcessState::RuleGridLevel)
		{
			GenerateRuleLevel(DeltaTime);
		} 
		else if (ProcessState == LevelProcessState::LowResLevel)
		{
			Command::CommandStack::GetInstance().ExecuteCommand(
				std::make_unique<Command::CreateLowResLevelCommand>(
					LowResLevelStateData,
					RuleLevelStateData
				)
			);
		}
	}

	void Level::Draw(Application::Renderer& Renderer)
	{
		if (ProcessState == LevelProcessState::RuleGridLevel)
		{
			DrawRuleGrid(Renderer);

			Renderer.SetDrawColor(32, 32, 32);
			Renderer.DrawRectangle({ 0.f, 0.f, static_cast<float>(GridSizeX), static_cast<float>(GridSizeY) });
		}
		else if (ProcessState == LevelProcessState::LowResLevel){
			DrawLowResLevel(Renderer);
		}
		
		DrawShortcutsWindow();
		DrawDebugTextWindow();
		DrawRulesInformationWindow();
	}

	void Level::GenerateRuleLevel(float DeltaTime)
	{
		if (!bGenerateLevel)
		{
			return;
		}

		if (SinceLastStepSeconds > WaitStepTimeSeconds)
		{
			const LevelGenerator::GeneratorActionType CurrentStep = LevelGenerator.Step();

			if (CurrentStep == LevelGenerator::GeneratorActionType::Failed)
			{
				NumberOfFailedAttempts++;

				Command::CommandStack::GetInstance().Clear();
				LevelGenerator = { RuleLevelStateData, LevelGenerator.MainRule };
			}

			if (CurrentStep == LevelGenerator::GeneratorActionType::Done)
			{
				ProcessState = LevelProcessState::LowResLevel;
			}

			SinceLastStepSeconds = 0.f;
			return;
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
			ResetMaze(LevelGenerator.MainRule);
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_PLUS))
		{
			if (GridSizeX < Configuration::MaxGridSize && GridSizeY < Configuration::MaxGridSize)
			{
				GridSizeX += 1;
				GridSizeY += 1;

				ResetMaze(LevelGenerator.MainRule);
			}
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_MINUS))
		{
			if (GridSizeX > Configuration::MinGridSize && GridSizeY > Configuration::MinGridSize)
			{
				GridSizeX -= 1;
				GridSizeY -= 1;

				ResetMaze(LevelGenerator.MainRule);
			}
		}
	}

	void Level::DrawRuleGrid(Application::Renderer& Renderer) const
	{
		const std::vector<std::vector<LevelGenerator::RuleLevelCell>>& grid = LevelGenerator.RuleLevelStateData.LevelGrid;

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

				if (LevelGenerator.RuleLevelStateData.CurrentCell->GetPosition().x == x && LevelGenerator.RuleLevelStateData.CurrentCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/" + std::to_string(0) + ".png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}

				if (LevelGenerator.RuleLevelStateData.StartCell->GetPosition().x == x && LevelGenerator.RuleLevelStateData.StartCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/start.png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}

				if (LevelGenerator.RuleLevelStateData.GoalCell->GetPosition().x == x && LevelGenerator.RuleLevelStateData.GoalCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/goal.png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}
			}
		}

		for (const auto& Pathway : LevelGenerator.RuleLevelStateData.InsertionPathways)
		{
			if (Pathway.empty())
			{
				continue;
			}

			int Index = 0;

			for (const LevelGenerator::RuleLevelCell* PathwayCell : Pathway)
			{
				const int X = PathwayCell->GetPosition().x;
				const int Y = PathwayCell->GetPosition().y;
				SDL_FRect TextureRect{
					static_cast<float>(X),
					static_cast<float>(Y),
					1.f,
					1.f
				};

				auto* image = Renderer.GetImage("resources/visited.png");
				Renderer.DrawTexture(image, TextureRect, 0);

				DrawElements(Renderer, TextureRect, grid[X][Y].GetElements());

				Index++;
			}
		}
	}

	void Level::DrawLowResLevel(Application::Renderer& Renderer) const
	{
		if (!LowResLevelStateData.HasGeneratedLevel())
		{
			return;
		}

		const std::vector<std::vector<LevelGenerator::LowResCell>>& Grid = LowResLevelStateData.LowResGrid;
		const int Width = LowResLevelStateData.GridSizeWidth;
		const int Height = LowResLevelStateData.GridSizeWidth;

		for (int X = 0; X < Width; X++)
		{
			for (int Y = 0; Y < Height; Y++)
			{
				const LevelGenerator::LowResCell& Cell = Grid[X][Y];

				SDL_Texture* image = nullptr;
				SDL_FRect TextureRect{
					static_cast<float>(X),
					static_cast<float>(Y),
					1.f,
					1.f
				};

				if (Cell.GetType() == LevelGenerator::LowResCellType::Room || Cell.GetType() == LevelGenerator::LowResCellType::Entrance)
				{
					Renderer.DrawTexture(Renderer.GetImage("resources/lowres-room.png"), TextureRect, 0);
				}
				else if (Cell.GetType() == LevelGenerator::LowResCellType::Start)
				{
					Renderer.DrawTexture(Renderer.GetImage("resources/lowres-start.png"), TextureRect, 0);
				}
				else if (Cell.GetType() == LevelGenerator::LowResCellType::Goal)
				{
					Renderer.DrawTexture(Renderer.GetImage("resources/lowres-goal.png"), TextureRect, 0);
				}
				
				if (Cell.GetType() == LevelGenerator::LowResCellType::Entrance)
				{
					image = Renderer.GetImage("resources/door.png");
				}

				DrawElements(Renderer, TextureRect, Cell.GetElements());

				Renderer.DrawTexture(image, TextureRect, 0);
			}
		}
	}

	void Level::DrawElements(Application::Renderer& Renderer, const SDL_FRect& TextureRect, const std::vector<std::shared_ptr<LevelElement::Element>>& Elements) const
	{
		for (const auto& Element : Elements)
		{
			if (Element->GetElementType() == LevelElement::ElementType::LockAndKey)
			{
				Renderer.DrawTexture(Renderer.GetImage("resources/key.png"), TextureRect, 0);
			}

			if (Element->GetElementType() == LevelElement::ElementType::MiniBoss)
			{
				Renderer.DrawTexture(Renderer.GetImage("resources/monster.png"), TextureRect, 0);
			}

			if (Element->GetElementType() == LevelElement::ElementType::Reward)
			{
				Renderer.DrawTexture(Renderer.GetImage("resources/reward.png"), TextureRect, 0);
			}

			if (Element->GetElementType() == LevelElement::ElementType::Trap)
			{
				Renderer.DrawTexture(Renderer.GetImage("resources/trap.png"), TextureRect, 0);
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

		ImGui::Text("Done: %s", ProcessState == LevelProcessState::Done ? "True" : "False");
		ImGui::Text("Number Of Failures: %d", NumberOfFailedAttempts);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Number Of Steps Taken: %d", LevelGenerator.PathwayCalculationData.NumberOfStepsTaken);
		ImGui::Text("Steps To Goal X: %d", LevelGenerator.PathwayCalculationData.StepsToGoalX);
		ImGui::Text("Steps To Goal Y: %d", LevelGenerator.PathwayCalculationData.StepsToGoalY);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Insertion One Steps: %llu", LevelGenerator.RuleLevelStateData.GetPathway(0).size());
		ImGui::Text("Insertion Two Steps: %llu", LevelGenerator.RuleLevelStateData.GetPathway(1).size());

		ImGui::End();
	}

	void Level::DrawRulesInformationWindow()
	{
		ImGui::SetNextWindowPos({ 16, static_cast<float>(Configuration::WindowHeight - 234 - 16) });
		ImGui::SetNextWindowSize({ 232, 234 }, 0);
		ImGui::Begin("Main Rule");

		ImGui::Text("Name:");
		ImGui::Text("%s", LevelGenerator.MainRule.GetName().c_str());
		ImGui::Text("Goal: %s", LevelGenerator.MainRule.GetGoalTypeToString().c_str());

		ImGui::Spacing();
		ImGui::Spacing();

		const auto [MinStepsOne, MaxStepsOne] = LevelGenerator.CalculateMinMaxSteps(LevelGenerator.MainRule.GetArcType(0));
		ImGui::Text("Insertion One");
		ImGui::Text("Element: %s", LevelGenerator.MainRule.GetElement(0).GetElementName().c_str());
		ImGui::Text("Path Length: %s", LevelGenerator.MainRule.GetArcType(0) == Cyclic::ArcType::Short ? "Short" : "Long");
		ImGui::Text("Steps: %d - %d", MinStepsOne, MaxStepsOne);

		ImGui::Spacing();
		ImGui::Spacing();

		const auto [MinStepsTwo, MaxStepsTwo] = LevelGenerator.CalculateMinMaxSteps(LevelGenerator.MainRule.GetArcType(1));
		ImGui::Text("Insertion Two");
		ImGui::Text("Element: %s", LevelGenerator.MainRule.GetElement(1).GetElementName().c_str());
		ImGui::Text("Path Length: %s", LevelGenerator.MainRule.GetArcType(1) == Cyclic::ArcType::Short ? "Short" : "Long");
		ImGui::Text("Steps: %d - %d", MinStepsTwo, MaxStepsTwo);

		ImGui::End();
	}

	void Level::ResetMaze(Cyclic::CyclicRule& MainRule)
	{
		Command::CommandStack::GetInstance().Clear();
		Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());

		ProcessState = LevelProcessState::RuleGridLevel;
		LevelGenerator = { RuleLevelStateData, MainRule };
		LowResLevelStateData = {};
		NumberOfFailedAttempts = 0;
	}
}
