#pragma once

#include "Application/Renderer.h"
#include "LevelGenerator/LowResLevel/LowResLevelStateData.h"
#include "LevelGenerator/RuleLevel/LevelGenerator.h"
#include "LevelGenerator/RuleLevel/RuleLevelStateData.h"

namespace LevelGeneration
{
	enum class LevelProcessState : uint8_t
	{
		RuleGridLevel = 0,
		LowResLevel = 1,
		HiResLevel = 2,
		Done = 3,
	};

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
		void GenerateRuleLevel(float DeltaTime);

		/**
		 * \brief Draws the grid of the maze/level
		 * \param Renderer Wrapper for SDL_Renderer
		 */
		void DrawRuleGrid(Application::Renderer& Renderer) const;

		void DrawLowResLevel(Application::Renderer& Renderer) const;

		void DrawElements(Application::Renderer& Renderer, const SDL_FRect& TextureRect, const std::vector<std::shared_ptr<LevelElement::Element>>& Elements) const;

		/**
		 * \brief Draws shortcut/input information in a ImGUI-window
		 */
		void DrawShortcutsWindow();

		/**
		 * \brief Draws debug data for the maze/level
		 */
		void DrawDebugTextWindow();

		void DrawRulesInformationWindow();

		/**
		 * \brief Replaces the maze object with a new and resets generation states and data
		 */
		void ResetMaze(Cyclic::CyclicRule& MainRule);
		
	private:
		bool bGenerateLevel{};
		int GridSizeX;
		int GridSizeY;
		int NumberOfFailedAttempts;
		float WaitStepTimeSeconds{};
		float SinceLastStepSeconds{};
		LevelProcessState ProcessState;

		Cyclic::CyclicRuleRepository& RuleRepository;
		LevelGenerator::LevelGenerator LevelGenerator;
		LevelGenerator::RuleLevelStateData RuleLevelStateData;
		LevelGenerator::LowResLevelStateData LowResLevelStateData;
	};
}
