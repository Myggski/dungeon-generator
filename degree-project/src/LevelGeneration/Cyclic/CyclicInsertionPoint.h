#pragma once

#include <cstdint>
#include <optional>

#include "CyclicRule.h"

namespace Cyclic
{

	enum class InsertionType : uint8_t
	{
		None = 0,
		LockKey = 1,
		Traps = 3,
		MiniBoss = 4,
		Reward = 5,
	};

	enum class ArcType : uint8_t
	{
		None = 0,
		Short = 1,
		Long = 2,
	};

	class CyclicInsertionPoint
	{
	public:
		CyclicInsertionPoint(ArcType ArcType = ArcType::None, InsertionType InsertionType = InsertionType::None);
		void AddMinorRule(std::optional<CyclicRule> Rule);
		bool HasMinorRule() const;

	private:
		ArcType ArcType;
		InsertionType InsertionType;
		std::optional<CyclicRule> MinorRule;
	};
}
