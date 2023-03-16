#pragma once
#include <SDL_rect.h>
#include <stack>
#include <unordered_map>

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

    const std::unordered_map<DirectionType, int> DirectionX
    {
        { DirectionType::North, 0 },
        { DirectionType::East, 1 },
        { DirectionType::South, 0 },
        { DirectionType::West, -1 },
    };

    const std::unordered_map<DirectionType, int> DirectionY
    {
        { DirectionType::North, -1 },
        { DirectionType::East, 0 },
        { DirectionType::South, 1 },
        { DirectionType::West, 0 },
    };

	class Maze
	{
	public:
        Maze(int Width, int Height) : Width(Width), Height(Height), CurrentCell(nullptr)
        {
            Grid = std::vector<std::vector<MazeCell>>();
            Grid.resize(Width);

	        for (int GridX = 0; GridX < Width; GridX++)
	        {
                Grid[GridX].resize(Height);

                for (int GridY = 0; GridY < Height; GridY++)
                {
                    Grid[GridX][GridY] = MazeCell(SDL_Point{GridX, GridY});
                }
	        }

            CurrentCell = &GetRandomStartCell();
            CurrentCell->SetVisited(true);

            VisitedStack.push(CurrentCell);
        }

        std::vector<std::vector<MazeCell>> GetGrid()
        {
            return Grid;
        }

        MazeCell& GetCurrent()
        {
            return *CurrentCell;
        }

        void CarvePassage()
        {
            if (VisitedStack.empty())
            {
                return;
            }

            const DirectionType RandomDirection = GetRandomDirection();

            if (RandomDirection != DirectionType::None)
            {
                const int NeighborX = CurrentCell->GetPosition().x + DirectionX.at(RandomDirection);
                const int NeighborY = CurrentCell->GetPosition().y + DirectionY.at(RandomDirection);
                MazeCell* NeighborCell = &Grid[NeighborX][NeighborY];

                CurrentCell->CarveEntrance(RandomDirection);

                NeighborCell->SetVisited(true);
                NeighborCell->CarveEntrance(OppositeDirection.at(RandomDirection));

                CurrentCell = NeighborCell;
                VisitedStack.push(CurrentCell);
            } else
            {
                VisitedStack.pop();

                if (!VisitedStack.empty())
                {
                    CurrentCell = VisitedStack.top();
                }
            }
        }

	private:
        DirectionType GetDirection(const MazeCell& CurrentCell, const MazeCell& NeighborCell) const
        {
            const int DirectionX = NeighborCell.GetPosition().x - CurrentCell.GetPosition().x;

            if (DirectionX != 0)
            {
                return DirectionX < 0
            		? DirectionType::West
            		: DirectionType::East;
            }

            return NeighborCell.GetPosition().y - CurrentCell.GetPosition().y < 0
        		? DirectionType::North
        		: DirectionType::South;
        }

        MazeCell& GetRandomStartCell()
        {
            // Which side of the grid should the start position be
            const DirectionType GridStartSide = static_cast<DirectionType>(Utils::RandomGenerator::GetInstance().GetRandom<int>(0, 3));

            switch (GridStartSide) {
            case DirectionType::North:
                return Grid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Width - 1)][0];
            case DirectionType::East:
                return Grid[Width - 1][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Height - 1)];
            case DirectionType::South:
                return Grid[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Width - 1)][Height - 1];
            case DirectionType::West:
            default:
                return Grid[0][Utils::RandomGenerator::GetInstance().GetRandom<int>(0, Height - 1)];
            }
        }

        DirectionType GetRandomDirection()
        {
            std::array<DirectionType, 4> Directions =
            {
                DirectionType::North,
                DirectionType::East,
                DirectionType::South,
                DirectionType::West
            };

            std::ranges::shuffle(Directions, Utils::RandomGenerator::GetInstance().GetEngine());
            DirectionType RandomDirection = DirectionType::None;

            for (const auto& CurrentDirection : Directions)
            {
                const int NeighborX = CurrentCell->GetPosition().x + DirectionX.at(CurrentDirection);
                const int NeighborY = CurrentCell->GetPosition().y + DirectionY.at(CurrentDirection);

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
        MazeCell* CurrentCell;
        std::vector<std::vector<MazeCell>> Grid;
        std::stack<MazeCell*> VisitedStack;

	};
}
