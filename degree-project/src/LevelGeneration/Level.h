#pragma once

#include "Application/Renderer.h"
#include "MazeGenerator/Maze.h"

namespace LevelGeneration
{
	class Level
	{
	public:
		Level(Cyclic::CyclicRule MainRule, int GridSizeX, int GridSizeY);

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
		void DrawMaze(Application::Renderer& Renderer);

		/**
		 * \brief Draws shortcut/input information in a ImGUI-window
		 */
		void DrawShortcutsWindow();

		/**
		 * \brief Draws debug data for the maze/level
		 */
		void DrawDebugTextWindow() const;

		/**
		 * \brief Replaces the maze object with a new and resets generation states and data
		 */
		void ResetMaze();
		
	private:
		bool bLevelDone{};
		bool bGenerateLevel{};
		int GridSizeX;
		int GridSizeY;
		int NumberOfFailedAttempts;
		float WaitStepTimeSeconds{};
		float SinceLastStepSeconds{};

		Cyclic::CyclicRule MainRule;
		MazeGenerator::Maze Maze;
	};
}
