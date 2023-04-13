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
#include "CommandStack/Commands/Level/CreateHiResLevelCommand.h"
#include <Utils/RandomGenerator.h>
#include "LevelGeneration/LevelGenerator/HiResLevel/Room.h"
#include "Application/Renderer.h"

namespace LevelGeneration
{
	Level::Level(Cyclic::CyclicRuleRepository& RuleRepository, int GridSizeX, int GridSizeY)
		: GridSizeX(GridSizeX),
		GridSizeY(GridSizeY),
		bGenerateLevel(false),
		bCheckpointReached(false),
		bIsLevelGenerated(false),
		RuleLevelStateData(GridSizeX, GridSizeY),
		NumberOfFailedAttempts(0),
		RuleRepository(RuleRepository),
		RuleLevelGenerator(RuleLevelStateData, RuleRepository.GetRandomRule()),
		CurrentProcessState(LevelProcessState::RuleGridLevel)
	{
		Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());
		RuleLevelGenerator.InitializeMaze();
	}

	void Level::Update(float DeltaTime)
	{
		CheckInput();
		GenerateLevel(DeltaTime);
	}

	void Level::Draw(Application::Renderer& Renderer)
	{
		float LevelBorderWidth = 0;
		float LevelBorderHeight = 0;
		float TileSize = Renderer.GetTileSize();

		if (CurrentProcessState == LevelProcessState::RuleGridLevel)
		{
			DrawRuleGrid(Renderer);

			LevelBorderWidth = static_cast<float>(RuleLevelStateData.GridWidth); 
			LevelBorderHeight = static_cast<float>(RuleLevelStateData.GridHeight);
		}
		else if (CurrentProcessState == LevelProcessState::LowResLevel)
		{
			DrawLowResLevel(Renderer);

			LevelBorderWidth = static_cast<float>(LowResLevelStateData.GridWidth);
			LevelBorderHeight = static_cast<float>(LowResLevelStateData.GridHeight);
		}
		else {
			DrawHiResLevel(Renderer);

			LevelBorderWidth = static_cast<float>(HiResLevelStateData.GridWidth * 7);
			LevelBorderHeight = static_cast<float>(HiResLevelStateData.GridHeight * 7);
			TileSize /= 2.f;
		}

		Renderer.SetDrawColor(155, 171, 178);
		Renderer.DrawRectangle({ 0.f, 0.f, LevelBorderWidth, LevelBorderHeight }, std::make_tuple(TileSize, TileSize));
		
		DrawShortcutsWindow();
		DrawDebugTextWindow();
		DrawRulesInformationWindow();
	}

	void Level::GenerateLevel(float DeltaTime)
	{
		if (!bGenerateLevel || bIsLevelGenerated)
		{
			return;
		}

		if (SinceLastStepSeconds > WaitStepTimeSeconds)
		{
			if (bCheckpointReached)
			{
				if (CurrentProcessState == LevelProcessState::LowResLevel)
				{
					CurrentProcessState = LevelProcessState::HiResLevel;
					bCheckpointReached = false;
				}

				if (CurrentProcessState == LevelProcessState::RuleGridLevel)
				{
					CurrentProcessState = LevelProcessState::LowResLevel;
					bCheckpointReached = false;
				}
			}

			if (CurrentProcessState == LevelProcessState::HiResLevel)
			{
				Command::CommandStack::GetInstance().ExecuteCommand(
					std::make_unique<Command::CreateHiResLevelCommand>(
						CurrentProcessState,
						HiResLevelStateData,
						LowResLevelStateData
					)
				);

				SinceLastStepSeconds = 0.f;
				bIsLevelGenerated = true;
			}

			if (CurrentProcessState == LevelProcessState::LowResLevel)
			{
				Command::CommandStack::GetInstance().ExecuteCommand(
					std::make_unique<Command::CreateLowResLevelCommand>(
						CurrentProcessState,
						LowResLevelStateData,
						RuleLevelStateData
					)
				);

				SinceLastStepSeconds = 0.f;
				bCheckpointReached = true;
			}

			if (CurrentProcessState == LevelProcessState::RuleGridLevel)
			{
				const LevelGenerator::GeneratorActionType CurrentStep = RuleLevelGenerator.Step();

				if (CurrentStep == LevelGenerator::GeneratorActionType::Failed)
				{
					NumberOfFailedAttempts++;

					Command::CommandStack::GetInstance().Clear();
					RuleLevelStateData = { GridSizeX, GridSizeY };
					RuleLevelGenerator = { RuleLevelStateData, RuleLevelGenerator.MainRule };
				}

				if (CurrentStep == LevelGenerator::GeneratorActionType::Done)
				{
					bCheckpointReached = true;
				}

				SinceLastStepSeconds = 0.f;
				return;
			}
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
			ResetLevelGeneration(RuleRepository.GetRandomRule());
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_P))
		{
			ResetLevelGeneration(RuleLevelGenerator.MainRule);
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_PLUS))
		{
			if (GridSizeX < Configuration::MaxGridSize && GridSizeY < Configuration::MaxGridSize)
			{
				GridSizeX += 1;
				GridSizeY += 1;

				ResetLevelGeneration(RuleLevelGenerator.MainRule);
			}
		}

		if (Application::Keyboard::GetInstance().IsKeyPressedOnce(SDL_SCANCODE_KP_MINUS))
		{
			if (GridSizeX > Configuration::MinGridSize && GridSizeY > Configuration::MinGridSize)
			{
				GridSizeX -= 1;
				GridSizeY -= 1;

				ResetLevelGeneration(RuleLevelGenerator.MainRule);
			}
		}
	}

	void Level::DrawRuleGrid(Application::Renderer& Renderer) const
	{
		const std::vector<std::vector<LevelGenerator::RuleLevelCell>>& grid = RuleLevelGenerator.RuleLevelStateData.LevelGrid;

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

				if (RuleLevelGenerator.RuleLevelStateData.CurrentCell->GetPosition().x == x && RuleLevelGenerator.RuleLevelStateData.CurrentCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/" + std::to_string(0) + ".png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}

				if (RuleLevelGenerator.RuleLevelStateData.StartCell->GetPosition().x == x && RuleLevelGenerator.RuleLevelStateData.StartCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/start.png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}

				if (RuleLevelGenerator.RuleLevelStateData.GoalCell->GetPosition().x == x && RuleLevelGenerator.RuleLevelStateData.GoalCell->GetPosition().y == y)
				{
					auto* image = Renderer.GetImage("resources/goal.png");
					Renderer.DrawTexture(image, TextureRect, 0);
				}
			}
		}

		for (const auto& Pathway : RuleLevelGenerator.RuleLevelStateData.InsertionPathways)
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

				Renderer.DrawTexture(Renderer.GetImage("resources/visited.png"), TextureRect, 0);

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
		const int Width = LowResLevelStateData.GridWidth;
		const int Height = LowResLevelStateData.GridWidth;

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
					if (Cell.IsStartCell())
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/lowres-start.png"), TextureRect, 0);
					}
					else if (Cell.IsGoalCell())
					{
						Renderer.DrawTexture(Renderer.GetImage("resources/lowres-goal.png"), TextureRect, 0);
					}
					else {
						Renderer.DrawTexture(Renderer.GetImage("resources/lowres-room.png"), TextureRect, 0);
					}
				}
				else {
					Renderer.DrawTexture(Renderer.GetImage("resources/lowres-empty.png"), TextureRect, 0);
				}

				DrawElements(Renderer, TextureRect, Cell.GetElements());

				if (Cell.GetType() == LevelGenerator::LowResCellType::Entrance)
				{
					Renderer.DrawTexture(Renderer.GetImage("resources/door.png"), TextureRect, 0);
				}
			}
		}
	}

	void Level::DrawHiResLevel(Application::Renderer& Renderer)
	{
		const int Width = HiResLevelStateData.GridWidth;
		const int Height = HiResLevelStateData.GridWidth;

		for (int X = 0; X < Width; X++)
		{
			for (int Y = 0; Y < Height; Y++)
			{
				HiResLevelStateData.HiResGrid[X][Y].Draw(Renderer, std::make_tuple(32.f, 32.f));
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

		ImGui::Text("Done: %s", CurrentProcessState == LevelProcessState::Done ? "True" : "False");
		ImGui::Text("Number Of Failures: %d", NumberOfFailedAttempts);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Number Of Steps Taken: %d", RuleLevelGenerator.PathwayCalculationData.NumberOfStepsTaken);
		ImGui::Text("Steps To Goal X: %d", RuleLevelGenerator.PathwayCalculationData.StepsToGoalX);
		ImGui::Text("Steps To Goal Y: %d", RuleLevelGenerator.PathwayCalculationData.StepsToGoalY);

		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Insertion One Steps: %llu", RuleLevelGenerator.RuleLevelStateData.GetPathway(0).size());
		ImGui::Text("Insertion Two Steps: %llu", RuleLevelGenerator.RuleLevelStateData.GetPathway(1).size());

		ImGui::End();
	}

	void Level::DrawRulesInformationWindow()
	{
		ImGui::SetNextWindowPos({ 16, static_cast<float>(Configuration::WindowHeight - 234 - 16) });
		ImGui::SetNextWindowSize({ 232, 234 }, 0);
		ImGui::Begin("Main Rule");

		ImGui::Text("Name:");
		ImGui::Text("%s", RuleLevelGenerator.MainRule.GetName().c_str());
		ImGui::Text("Goal: %s", RuleLevelGenerator.MainRule.GetGoalTypeToString().c_str());

		ImGui::Spacing();
		ImGui::Spacing();

		const auto [MinStepsOne, MaxStepsOne] = RuleLevelGenerator.CalculateMinMaxSteps(RuleLevelGenerator.MainRule.GetArcType(0));
		ImGui::Text("Insertion One");
		ImGui::Text("Element: %s", RuleLevelGenerator.MainRule.GetElement(0).GetElementName().c_str());
		ImGui::Text("Path Length: %s", RuleLevelGenerator.MainRule.GetArcType(0) == Cyclic::ArcType::Short ? "Short" : "Long");
		ImGui::Text("Steps: %d - %d", MinStepsOne, MaxStepsOne);

		ImGui::Spacing();
		ImGui::Spacing();

		const auto [MinStepsTwo, MaxStepsTwo] = RuleLevelGenerator.CalculateMinMaxSteps(RuleLevelGenerator.MainRule.GetArcType(1));
		ImGui::Text("Insertion Two");
		ImGui::Text("Element: %s", RuleLevelGenerator.MainRule.GetElement(1).GetElementName().c_str());
		ImGui::Text("Path Length: %s", RuleLevelGenerator.MainRule.GetArcType(1) == Cyclic::ArcType::Short ? "Short" : "Long");
		ImGui::Text("Steps: %d - %d", MinStepsTwo, MaxStepsTwo);

		ImGui::End();
	}

	void Level::ResetLevelGeneration(Cyclic::CyclicRule& MainRule)
	{
		Command::CommandStack::GetInstance().Clear();
		Utils::RandomGenerator::GetInstance().SetSeed(Utils::RandomGenerator::GetInstance().GetSeed());

		CurrentProcessState = LevelProcessState::RuleGridLevel;
		RuleLevelStateData = { GridSizeX, GridSizeY };
		RuleLevelGenerator = { RuleLevelStateData, MainRule };
		LowResLevelStateData = {};
		NumberOfFailedAttempts = 0;
		bCheckpointReached = false;
		bIsLevelGenerated = false;
	}
}
