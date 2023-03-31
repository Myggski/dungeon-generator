#pragma once

#include "LevelCell.h"
#include "DirectionType.h"
#include "LevelStateData.h"
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

namespace LevelGenerator
{

	class LevelGenerator
	{
	public:
        LevelGenerator(Cyclic::CyclicRule& MainRule, int Width, int Height);

        inline LevelGenerator& operator=(LevelGenerator&& Other) noexcept
        {
            MainRule = Other.MainRule;
            StateData = { Other.StateData.GridWidth, Other.StateData.GridHeight };

            InitializeMaze();

            return *this;
        }

        /// <summary>
        /// Setting up the start and goal cells. 
        /// TODO: The goal should be dependent on the main rule.
        /// If there are any short paths, put the goal closer to the start
        /// </summary>
        void InitializeMaze();
        GeneratorActionType Step();
        std::pair<int, int> CalculateMinMaxSteps(Cyclic::ArcType ArcType) const;

	private:
        /**
         * \brief Returns the neighbor cell by direction
         * \param From Where to go from
         * \param Direction The direction to go to
         * \return Returns the neighboring cell, if it's not a valid direction it returns nullptr
         */
        const LevelCell* GetNeighborCell(LevelCell* From, DirectionType Direction) const;

        /**
         * \brief Checks for unvisited neighbors, if there are any, pick one of them and move towards it
         */
        void TryCarvePassage();

        /**
         * \brief Calculating and weighting the paths by checking data relating to the pathway
         * \return The next direction to move towards
         */
        DirectionType CalculateNextDirection() const;

        /// <summary>
        /// Returning the horisontal and vertical direction from one cell to another
        /// TODO: Try to find another data structure instead of std::pair
        /// </summary>
        /// <param name="CurrentCell"></param>
        /// <param name="NeighborCell"></param>
        /// <returns></returns>
        NavigationalDirections GetCellDirection(const LevelCell& CurrentCell, const LevelCell& NeighborCell) const;

        /**
         * \brief Setting up 
         */
        void InitializeStateData();

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
        LevelCell* GetRandomEdgeCell(DirectionType Direction);

        /**
         * \brief Checks the surroundings of the current cell and see if there are any unvisited cells
         * \return A list of available directions
         */
        std::vector<DirectionType> GetAvailableDirections() const;

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
        static bool IsOutOfBound(const LevelStateData& StateData, int PositionX, int PositionY);

	private:
        LevelStateData StateData;
        PathwayData PathwayCalculationData;
        Cyclic::CyclicRule& MainRule;

        friend class LevelGeneration::Level;
	};
}
