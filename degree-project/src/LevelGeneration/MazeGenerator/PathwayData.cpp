#include "PathwayData.h"

namespace MazeGenerator
{
	PathwayData::PathwayData()
		: NumberOfStepsTaken(0),
		StepsToGoalX(0),
		StepsToGoalY(0),
		MinSteps(0),
		MaxSteps(0),
		DirectionsToGoal({ DirectionType::None, DirectionType::None }) { }
}