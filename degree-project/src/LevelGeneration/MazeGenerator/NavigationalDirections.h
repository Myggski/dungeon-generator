#pragma once

#include <unordered_map>

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

        // Don't want to favorise a direction
        DirectionType GetRandomDirection() const
        {
            return Utils::RandomGenerator::GetInstance().GetRandom(0, 1) < 1
                ? Horizontal
                : Vertical;
        }
    private:
        DirectionType Horizontal;
        DirectionType Vertical;
    };
}
