#pragma once

#include <cstdint>

enum class LevelProcessState : uint8_t
{
	RuleGridLevel = 0,
	LowResLevel = 1,
	HiResLevel = 2,
	Done = 3,
};