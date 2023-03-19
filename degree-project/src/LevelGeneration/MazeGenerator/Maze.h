#pragma once
#include <bitset>
#include <iostream>
#include <SDL_rect.h>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "MazeCell.h"
#include "utils/RandomGenerator.h"
#include "DirectionType.h"
#include "PathwayData.h"

namespace MazeGenerator
{
    const std::unordered_map<DirectionType, DirectionType> OppositeDirection
    {
        { DirectionType::North, DirectionType::South },
        { DirectionType::East, DirectionType::West },
        { DirectionType::South, DirectionType::North },
        { DirectionType::West, DirectionType::East },
    };

    const std::unordered_map<DirectionType, int> DirectionToGridStepX
    {
        { DirectionType::North, 0 },
        { DirectionType::East, 1 },
        { DirectionType::South, 0 },
        { DirectionType::West, -1 },
    };

    const std::unordered_map<DirectionType, int> DirectionToGridStepY
    {
        { DirectionType::North, -1 },
        { DirectionType::East, 0 },
        { DirectionType::South, 1 },
        { DirectionType::West, 0 },
    };

	class Maze
	{
	public:
        Maze(int Width, int Height)
			: Width(Width),
			Height(Height),
			bGoalHasBeenReached(false),
			CurrentCell(nullptr),
			StartCell(nullptr),
			GoalCell(nullptr)
        {
	        Grid.resize(Width);

	        for (int GridX = 0; GridX < Width; GridX++)
	        {
		        Grid[GridX].resize(Height);

		        for (int GridY = 0; GridY < Height; GridY++)
		        {
			        Grid[GridX][GridY] = MazeCell(SDL_Point{GridX, GridY});
		        }
	        }

            InitializePath();
        }

        std::vector<std::vector<MazeCell>> GetGrid()
        {
            return Grid;
        }

        MazeCell& GetCurrentCell() const
        {
            return *CurrentCell;
        }

        MazeCell& GetStartCell() const
        {
            return *StartCell;
        }

        MazeCell& GetGoalCell() const
        {
            return *GoalCell;
        }

        std::vector<MazeCell*> GetPathway()
        {
            return Pathway;
        }

        void CarveCellPassage()
        {
            if (VisitedStack.empty())
            {
                return;
            }

            const bool bNewPathCreated = TryMakePathToNeighbor();

            // Backtrack if there's no new path that has been created
            if (!bNewPathCreated)
            {
                BacktrackVisitedPath();
                return;
            }

            // If a new path has been created, check if goal has been reached
            if (CurrentCell == GoalCell)
            {
                SetGoalReached();
            }   
        }

	private:
        /// <summary>
        /// When no more path
        /// </summary>
        void BacktrackVisitedPath()
        {
            // If goal has been reached, save pathway on the way back
            if (bGoalHasBeenReached)
            {
                Pathway.emplace_back(VisitedStack.top());
            }

            // Go back to the visited stack
            VisitedStack.pop();

            if (!VisitedStack.empty())
            {
                CurrentCell = VisitedStack.top();
            }
        }

        /// <summary>
        /// Preparing the pathway vector and set the number of steps value 
        /// </summary>
        void SetGoalReached()
        {
            bGoalHasBeenReached = true;
            Pathway.reserve(VisitedStack.size());
            PathwayCalculationData.NumberOfStepsTaken = VisitedStack.size();
        }

        /// <summary>
        /// Trying to find available neighbors and create a path between current cell to neighbor cell
        /// </summary>
        /// <returns>True if valid neighbor is found, false if there's no available paths to go</returns>
        bool TryMakePathToNeighbor()
        {
            const DirectionType AvailableDirection = GetAvailableRandomDirection();

            if (AvailableDirection == DirectionType::None || bGoalHasBeenReached)
            {
                return false;
            }

            // Carve path for current cell to neighbor cell
            CurrentCell->CarveEntrance(AvailableDirection);

            // Get Neighbor position
            const int NeighborX = CurrentCell->GetPosition().x + DirectionToGridStepX.at(AvailableDirection);
            const int NeighborY = CurrentCell->GetPosition().y + DirectionToGridStepY.at(AvailableDirection);
            
            // Carve path for neighbor cell to current cell
            MazeCell* const NeighborCell = &Grid[NeighborX][NeighborY];
            NeighborCell->CarveEntrance(OppositeDirection.at(AvailableDirection));
            NeighborCell->SetVisited(true);

            // Set neighbor to current cell
            CurrentCell = NeighborCell;

            // Push current cell into the visited stack
            VisitedStack.push(CurrentCell);
            PathwayCalculationData.NumberOfStepsTaken = VisitedStack.size();

            return true;
        }

        /// <summary>
        /// Returning the horisontal and vertical direction from one cell to another
        /// TODO: Try to find another data structure instead of std::pair
        /// </summary>
        /// <param name="CurrentCell"></param>
        /// <param name="NeighborCell"></param>
        /// <returns></returns>
        std::pair<DirectionType, DirectionType> GetCellDirection(const MazeCell& CurrentCell, const MazeCell& NeighborCell) const
        {
            const int DirectionX = NeighborCell.GetPosition().x - CurrentCell.GetPosition().x;
            const int DirectionY = NeighborCell.GetPosition().y - CurrentCell.GetPosition().y;

            DirectionType DirectionHorisontal = DirectionType::None;
            DirectionType DirectionVertical = DirectionType::None;

            if (DirectionX != 0)
            {
                DirectionHorisontal = DirectionX < 0
            		? DirectionType::West
            		: DirectionType::East;
            }

            if (DirectionY != 0)
                DirectionVertical = DirectionY < 0
        		? DirectionType::North
        		: DirectionType::South;

            return { DirectionHorisontal, DirectionVertical };
        }

        /*
         * 
         */
        
        /// <summary>
        /// Setting up the start and goal cells. 
        /// TODO: The goal should be dependent on the main rule.
        /// If there are any short paths, put the goal closer to the start
        /// </summary>
        void InitializePath()
        {
            // Which side of the grid should the start position be
            const DirectionType StartPositionDirection = GetRandomDirection();

            StartCell = GetRandomEdgeCell(StartPositionDirection);
            StartCell->SetVisited(true);

            // Set the goal cell to be at the opposite site of the start cell
            GoalCell = GetRandomEdgeCell(OppositeDirection.at(StartPositionDirection));

            CurrentCell = StartCell;
            VisitedStack.push(CurrentCell);

            PathwayCalculationData = PathwayData();

            std::pair<DirectionType, DirectionType> directions = GetCellDirection(*CurrentCell, *GoalCell);
            PathwayCalculationData.DirectionToGoalX = directions.first;
            PathwayCalculationData.DirectionToGoalY = directions.second;

            CalculateStepsLeft();

            PathwayCalculationData.MinSteps =  PathwayCalculationData.StepsToGoalX + PathwayCalculationData.StepsToGoalY;
            PathwayCalculationData.MaxSteps = PathwayCalculationData.MinSteps * 1.2;
        }

        void CalculateStepsLeft()
        {
            PathwayCalculationData.StepsToGoalX = abs(CurrentCell->GetPosition().x - GoalCell->GetPosition().x);
            PathwayCalculationData.StepsToGoalY = abs(CurrentCell->GetPosition().y - GoalCell->GetPosition().y);
        }

        // Get random maze cell from the edge of the grid
        MazeCell* GetRandomEdgeCell(DirectionType Direction)
        {
            switch (Direction)
            {
            case DirectionType::North:
                return &Grid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Width - 1)][0];
            case DirectionType::East:
                return &Grid[Width - 1][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Height - 1)];
                break;
            case DirectionType::South:
                return &Grid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Width - 1)][Height - 1];
            case DirectionType::West:
            default:
                return &Grid[0][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Height - 1)];
            }
        }

        std::array<DirectionType, 4> GetRandomDirections() const
        {
            std::array<DirectionType, 4> Directions =
            {
                DirectionType::North,
                DirectionType::East,
                DirectionType::South,
                DirectionType::West
            };

            std::ranges::shuffle(Directions, Utils::RandomGenerator::GetInstance().GetEngine());

            return Directions;
        }

        DirectionType GetRandomDirection() const
        {
            return GetRandomDirections()[0];
        }

        DirectionType GetAvailableRandomDirection() const
        {
	        const std::array<DirectionType, 4> Directions = GetRandomDirections();
            DirectionType RandomDirection = DirectionType::None;

            for (const auto& CurrentDirection : Directions)
            {
                const int NeighborX = CurrentCell->GetPosition().x + DirectionToGridStepX.at(CurrentDirection);
                const int NeighborY = CurrentCell->GetPosition().y + DirectionToGridStepY.at(CurrentDirection);

                if (IsOutOfBound(NeighborX, NeighborY))
                {
                    continue;
                }

                if (Grid[NeighborX][NeighborY].IsVisited())
                {
	                continue;
                }

                RandomDirection = CurrentDirection;
            }

            return RandomDirection;
        }  

        bool IsOutOfBound(int PositionX, int PositionY) const
        {
            return (PositionX < 0 || PositionX > Width - 1) || (PositionY < 0 || PositionY > Height - 1);
        }

	private:
        int Width;
        int Height;
        bool bGoalHasBeenReached;
        MazeCell* CurrentCell;
        MazeCell* StartCell;
        MazeCell* GoalCell;
        std::vector<std::vector<MazeCell>> Grid;
        std::stack<MazeCell*> VisitedStack;
        std::vector<MazeCell*> Pathway;
        PathwayData PathwayCalculationData;

	};
}
