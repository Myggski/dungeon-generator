#include "MiniBossProperty.h"

namespace LevelElement
{
	MiniBossProperty::MiniBossProperty(DifficultyType Difficulty, int NumberOfRoomsPatrol)
		: Difficulty(Difficulty),
		  NumberOfRoomsPatrol(NumberOfRoomsPatrol) { }

	DifficultyType MiniBossProperty::GetDifficulty() const
	{
		return Difficulty;
	}

	int MiniBossProperty::GetNumberOfRoomsPatrol() const
	{
		return NumberOfRoomsPatrol;
	}
}