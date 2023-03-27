#pragma once

#include "MazeCell.h"
#include "DirectionType.h"
#include "MazeStateData.h"
#include "PathwayData.h"
#include "NavigationalDirections.h"
#include "LevelGeneration/Cyclic/CyclicRule.h"

namespace LevelGeneration
{
    class Level;
}

namespace Command
{
	class CarvePassageCommand;
}

namespace MazeGenerator
{

	class Maze
	{
	public:
        Maze(Cyclic::CyclicRule& MainRule, int Width, int Height);
        inline void operator=(const Maze& Other)
        {
            MainRule = Other.MainRule;
            StateData = Other.StateData;
            PathwayCalculationData = Other.PathwayCalculationData;
        }

        MazeActionType Step();

	private:
        /**
         * \brief Returns the neighbor cell by direction
         * \param From Where to go from
         * \param Direction The direction to go to
         * \return Returns the neighboring cell, if it's not a valid direction it returns nullptr
         */
        const MazeCell* GetNeighborCell(MazeCell* From, DirectionType Direction) const;

        /**
         * \brief Checks for unvisited neighbors, if there are any, pick one of them and move towards it
         */
        void TryCarvePassage();

        /**
         * \brief Calculating and weighting the paths by checking data relating to the pathway
         * \return The next direction to move towards
         */
        DirectionType CalculateNextDirection() const;

        /**
         * \brief TODO: Remove this?
         * \return Returns the direction that has the highest number of steps
         */
        DirectionType GetDirectionFurthestGoal() const;

        /// <summary>
        /// Returning the horisontal and vertical direction from one cell to another
        /// TODO: Try to find another data structure instead of std::pair
        /// </summary>
        /// <param name="CurrentCell"></param>
        /// <param name="NeighborCell"></param>
        /// <returns></returns>
        NavigationalDirections GetCellDirection(const MazeCell& CurrentCell, const MazeCell& NeighborCell) const;

        /**
         * \brief Setting up 
         */
        void InitializeStateData();

        /// <summary>
        /// Setting up the start and goal cells. 
        /// TODO: The goal should be dependent on the main rule.
        /// If there are any short paths, put the goal closer to the start
        /// </summary>
        void InitializeMaze();

        /**
         * \brief Changes the CurrentPathIndex and starts to save the path in another array
         */
        void CreateNewPath();

        /**
         * \brief Recalculates the number of steps left, the direction and number of steps taken towards goal
         */
        void CalculateStepsLeft();

        /**
         * \brief Get random maze cell from the edge of the grid
         * \param Direction Which side it should take the cell from
         * \return A MazeCell that's at the edge of the grid
         */
        MazeCell* GetRandomEdgeCell(DirectionType Direction);

        /**
         * \brief Checks the surroundings of current cell and see if there are any unvisited cells
         * \return A random direction that is not yet visited
         */
        std::vector<DirectionType> GetAvailableRandomDirections() const;

        /**
         * \brief Checks if the wanted position in the grid is out of bound
         * \param StateData State of the maze
         * \param PositionX X-axis position in the 2D grid
         * \param PositionY Y-axis position in the 2D grid
         * \return Returns true if it's out of bound, else it returns false
         */
        static bool IsOutOfBound(const MazeStateData& StateData, int PositionX, int PositionY);

	private:
        MazeStateData StateData;
        PathwayData PathwayCalculationData;
        Cyclic::CyclicRule& MainRule;

        friend class LevelGeneration::Level;
	};
}
