#pragma once

#include <cstdint>

#include "ElementProperty.h"

namespace LevelElement
{
	enum class DifficultyType : uint8_t
	{
		Easy = 1 << 1,
		Medium = 1 << 2,
		Hard = 1 << 3
	};

	struct MiniBossProperty : public ElementPropertyCRTP<MiniBossProperty>
	{
	public:
		MiniBossProperty(DifficultyType Difficulty, int NumberOfRoomsPatrol = 0);

		DifficultyType GetDifficulty() const;
		int GetNumberOfRoomsPatrol() const;

	private:
		DifficultyType Difficulty;
		int NumberOfRoomsPatrol;
	};
}
