#pragma once

#include <set>

#include "Application/Renderer.h"
#include "LevelGenerator/LowResLevel/LowResLevelStateData.h"
#include "LevelGenerator/HiResLevel/HiResLevelStateData.h"
#include "LevelGenerator/RuleLevel/RuleLevelGenerator.h"
#include "LevelGenerator/RuleLevel/RuleLevelStateData.h"
#include "LevelGenerator/ProcessState.h"

namespace LevelGeneration
{
	class Level
	{
	public:
		Level(Cyclic::CyclicRuleRepository& RuleRepository, int GridSizeX, int GridSizeY);

		/**
		 * \brief Checking for inputs and generates a level
		 * \param DeltaTime Time since last frame
		 */
		void Update(float DeltaTime);

		/**
		 * \brief Drawing the level and debug information
		 * \param Renderer Wrapper for SDL_Renderer
		 */
		void Draw(Application::Renderer& Renderer);

	private:
		/**
		 * \brief Updates/changes the maze if any keys are being pressed
		 */
		void CheckInput();

		/**
		 * \brief Generates the level one step each frame
		 * \param DeltaTime Time since last frame
		 */
		void GenerateLevel(float DeltaTime);

		/**
		 * \brief Draws the grid of the maze/level
		 * \param Renderer Wrapper for SDL_Renderer
		 */
		void DrawRuleGrid(Application::Renderer& Renderer) const;

		void DrawLowResLevel(Application::Renderer& Renderer) const;

		void DrawHiResLevel(Application::Renderer& Renderer);

		void DrawElements(Application::Renderer& Renderer, const SDL_FRect& TextureRect, const std::vector<std::shared_ptr<LevelElement::Element>>& Elements) const;

		void DrawToolsWindow();

		/**
		 * \brief Replaces the maze object with a new and resets generation states and data
		 */
		void ResetLevelGeneration(Cyclic::CyclicRule& MainRule);
		
	private:
		bool bGenerateLevel;
		bool bCheckpointReached;
		bool bIsLevelGenerated;
		int GridSizeX;
		int GridSizeY;
		int NumberOfFailedAttempts;
		float WaitStepTimeSeconds{};
		float SinceLastStepSeconds{};
		LevelProcessState CurrentProcessState;

		Cyclic::CyclicRuleRepository& RuleRepository;
		LevelGenerator::RuleLevelGenerator RuleLevelGenerator;
		LevelGenerator::RuleLevelStateData RuleLevelStateData;
		LevelGenerator::LowResLevelStateData LowResLevelStateData;
		LevelGenerator::HiResLevelStateData HiResLevelStateData;
	};
}
