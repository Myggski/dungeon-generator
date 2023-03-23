#pragma once

#include <stack>
#include <unordered_map>

#include "MazeCell.h"
#include "DirectionType.h"
#include "MazeStateData.h"
#include "PathwayData.h"
#include "NavigationalDirections.h"

namespace Command
{
	class CarvePassageCommand;
}

namespace MazeGenerator
{

	class Maze
	{
	public:
        Maze(int Width, int Height);

        MazeActionType Step();
        void DrawDebugText();
        std::vector<MazeCell*> GetPathway();

        // Probably temporary getter funcions for debugging
        std::vector<std::vector<MazeCell>> GetGrid();
        MazeCell& GetCurrentCell() const;
        MazeCell& GetStartCell() const;
        MazeCell& GetGoalCell() const;
        MazeCell* GetCell(int PositionX, int PositionY);

	private:
        const MazeCell* GetNeighborCell(MazeCell* From, DirectionType Direction) const;
        
        /// <summary>
        /// Trying to find available neighbors and create a path between current cell to neighbor cell
        /// </summary>
        /// <returns>True if valid neighbor is found, false if there's no available paths to go</returns>
        void TryCarvePassage();

        DirectionType CalculateNextDirection() const;
        
        DirectionType GetDirectionFurthestGoal() const;

        /// <summary>
        /// Returning the horisontal and vertical direction from one cell to another
        /// TODO: Try to find another data structure instead of std::pair
        /// </summary>
        /// <param name="CurrentCell"></param>
        /// <param name="NeighborCell"></param>
        /// <returns></returns>
        NavigationalDirections GetCellDirection(const MazeCell& CurrentCell, const MazeCell& NeighborCell) const;

        static bool IsOutOfBound(const MazeStateData& StateData, int PositionX, int PositionY);

        void InitializeStateData();

        /// <summary>
        /// Setting up the start and goal cells. 
        /// TODO: The goal should be dependent on the main rule.
        /// If there are any short paths, put the goal closer to the start
        /// </summary>
        void InitializeMaze();

        void CreateNewPath();

        void CalculateStepsLeft();

        // Get random maze cell from the edge of the grid
        MazeCell* GetRandomEdgeCell(DirectionType Direction);

        std::array<DirectionType, 4> GetRandomDirections() const;

        DirectionType GetRandomDirection() const;

        std::vector<DirectionType> GetAvailableRandomDirections() const;

	private:
        MazeStateData StateData;
        PathwayData PathwayCalculationData;
	};
}
