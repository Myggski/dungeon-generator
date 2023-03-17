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

	        SetStartAndGoalCells();

	        CurrentCell = StartCell;
	        CurrentCell->SetVisited(true);
            
	        VisitedStack.push(CurrentCell);
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

        void CarveCellPassage()
        {
            if (VisitedStack.empty())
            {
                return;
            }

            const DirectionType AvailableDirection = GetAvailableRandomDirection();

            // If there are any neighboring maze cells left that are unvisited
            if (AvailableDirection != DirectionType::None && !bGoalHasBeenReached)
            {
                const int NeighborX = CurrentCell->GetPosition().x + DirectionToGridStepX.at(AvailableDirection);
                const int NeighborY = CurrentCell->GetPosition().y + DirectionToGridStepY.at(AvailableDirection);
                MazeCell* NeighborCell = &Grid[NeighborX][NeighborY];

                CurrentCell->CarveEntrance(AvailableDirection);

                NeighborCell->SetVisited(true);
                NeighborCell->CarveEntrance(OppositeDirection.at(AvailableDirection));

                CurrentCell = NeighborCell;
                VisitedStack.push(CurrentCell);

                std::cout << "Total steps: " << VisitedStack.size() << " StepsX: " << abs(CurrentCell->GetPosition().x - GoalCell->GetPosition().x) << " StepsY: " << abs(CurrentCell->GetPosition().y - GoalCell->GetPosition().y) << "\n";

                if (CurrentCell == GoalCell)
                {
                    bGoalHasBeenReached = true;
				}

                return;
            }

            if (!VisitedStack.empty())
            {
                if (bGoalHasBeenReached)
                {
                Pathway.push_back(VisitedStack.top());
                }

                VisitedStack.pop();

                if (!VisitedStack.empty())
            {
                    CurrentCell = VisitedStack.top();
                }
            }
        }

        std::vector<MazeCell*> GetPathway()
        {
            return Pathway;
        }

	private:
        std::pair<DirectionType, DirectionType> GetCellDirection(const MazeCell& CurrentCell, const MazeCell& NeighborCell)
        {
            const int DirectionX = NeighborCell.GetPosition().x - CurrentCell.GetPosition().x;
            const int DirectionY = NeighborCell.GetPosition().y - CurrentCell.GetPosition().y < 0;

            DirectionType DirectionHorisontal = DirectionType::None;
            DirectionType DirectionVertical = DirectionType::None;

            if (DirectionX != 0)
            {
                DirectionHorisontal = DirectionX < 0
            		? DirectionType::West
            		: DirectionType::East;
            }

            if (DirectionY != 0)
				DirectionVertical =  DirectionY < 0
        		? DirectionType::North
        		: DirectionType::South;

            return { DirectionHorisontal, DirectionVertical };
        }

        /*
         * TODO: The goal should be dependent on the main rule.
         * If there are any short paths, put the goal closer to the start
         */
        void SetStartAndGoalCells()
        {
            // Which side of the grid should the start position be
            const DirectionType StartPositionDirection = GetRandomDirection();

            StartCell = GetRandomEdgeCell(StartPositionDirection);
            GoalCell = GetRandomEdgeCell(OppositeDirection.at(StartPositionDirection));
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

	};
}
