#pragma once

#include <unordered_map>
#include <array>

#include "DirectionType.h"
#include "utils/RandomGenerator.h"

namespace MazeGenerator
{
    const std::unordered_map<DirectionType, DirectionType> OppositeDirection
    {
        { DirectionType::None, DirectionType::None },
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

    struct NavigationalDirections
    {
    public:
        NavigationalDirections(DirectionType Horizontal, DirectionType Vertical)
            : Horizontal(Horizontal),
            Vertical(Vertical) {}

        DirectionType GetHorizontal() const
        {
            return Horizontal;
        }

        DirectionType GetVertical() const
        {
            return Vertical;
        }

        static std::array<DirectionType, 4> GetRandomDirections()
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

        static DirectionType GetRandomDirection()
        {
            return GetRandomDirections()[0];
        }
    private:
        DirectionType Horizontal;
        DirectionType Vertical;
    };
}
