#pragma once

#include "NavigationalDirections.h"

namespace LevelGenerator
{
    struct PathwayData
    {
    public:
        PathwayData();

    public:
        int NumberOfStepsTaken;
        int StepsToGoalX;
        int StepsToGoalY;
        int MinSteps;
        int MaxSteps;
        NavigationalDirections DirectionsToGoal;
    };
}