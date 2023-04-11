#pragma once

#include "DirectionType.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelCell.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/RuleLevelStateData.h"
#include "PathwayData.h"
#include "NavigationalDirections.h"
#include "LevelGeneration/Cyclic/CyclicRule.h"

namespace Command
{
    class CreateLowResLevelCommand;
}

namespace LevelGenerator
{
    class LevelGenerator
    {
    public:
        LevelGenerator(RuleLevelStateData& RuleLevelStateData, Cyclic::CyclicRule MainRule);
        LevelGenerator& operator=(LevelGenerator&& Other) noexcept;

        /// <summary>
        /// Setting up the start and goal cells. 
        /// TODO: The goal should be dependent on the main rule.
        /// If there are any short paths, put the goal closer to the start
        /// </summary>
        void InitializeMaze();
        GeneratorActionType Step();
        std::tuple<int, int> CalculateMinMaxSteps(Cyclic::ArcType ArcType) const;
        int GetGridWidth() const;
        int GetGridHeight() const;

	private:

        /**
         * \brief Checks for unvisited neighbors, if there are any, pick one of them and move towards it
         */
        void TryCarvePassage();

        std::vector<std::tuple<DirectionType, float>> GetWeightedDirections(const std::vector<DirectionType>& AvailableDirections);

        /**
         * \brief Calculating and weighting the paths by checking data relating to the pathway
         * \return The next direction to move towards
         */
        DirectionType CalculateNextDirection();

        /// <summary>
        /// Returning the horizontal and vertical direction from one cell to another
        /// TODO: Try to find another data structure instead of std::pair
        /// </summary>
        /// <param name="CurrentCell"></param>
        /// <param name="NeighborCell"></param>
        /// <returns></returns>
        NavigationalDirections GetCellDirection(const RuleLevelCell& CurrentCell, const RuleLevelCell& NeighborCell) const;

        /**
         * \brief Recalculates the number of steps left, the direction and number of steps taken towards goal
         */
        void CalculateStepsLeft();

        /**
         * \brief Get random maze cell from the edge of the grid
         * \param Direction Which side it should take the cell from
         * \return A MazeCell that's at the edge of the grid
         */
        RuleLevelCell* GetRandomEdgeCell(DirectionType Direction);
        
        RuleLevelCell* GetRandomGoalCell(DirectionType Direction);

        std::vector<std::vector<RuleLevelCell>>& GetLevelGrid();

	private:
        PathwayData PathwayCalculationData;
        Cyclic::CyclicRule MainRule;
        RuleLevelStateData& RuleLevelStateData;

        friend class LevelGeneration::Level;
        friend class Command::CreateLowResLevelCommand;
	};
}
