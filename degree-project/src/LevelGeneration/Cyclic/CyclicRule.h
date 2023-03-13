#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <SDL_rect.h>
#include <string>

namespace Cyclic
{
	class CyclicRuleRepository;
	class CyclicInsertionPoint;

	enum class GoalType : uint8_t
	{
		Treasure = 0x00,
		SecretDocuments = 0x01,
		KillTarget = 0x02,
	};

	inline GoalType operator|(GoalType lhs, GoalType rhs)
	{
		return static_cast<GoalType>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	class CyclicRule
	{
	public:
		CyclicRule(std::string RuleName, std::array<CyclicInsertionPoint&, 2> InsertionPoints, GoalType GoalType);

	private:
		std::string RuleName;
		GoalType GoalType;
		std::array<CyclicInsertionPoint&, 2> InsertionPoints;

		friend CyclicRuleRepository;
	};
}
