#include "DirectionType.h"

namespace MazeGenerator
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
        DirectionType DirectionToGoalX;
        DirectionType DirectionToGoalY;
    };
}