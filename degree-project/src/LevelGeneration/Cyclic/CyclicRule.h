#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <SDL_rect.h>
#include <string>

#include "CyclicInsertionPoint.h"

namespace Cyclic
{
	class CyclicRuleRepository;

	enum class GoalType : uint8_t
	{
		Treasure = 1 << 0,
		SecretDocuments = 1 << 1,
		KillTarget = 1 << 2,
	};

	inline GoalType operator|(GoalType lhs, GoalType rhs)
	{
		return static_cast<GoalType>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline GoalType operator&(GoalType lhs, GoalType rhs)
	{
		return static_cast<GoalType>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}

	class CyclicRule
	{
	public:
		CyclicRule(std::string RuleName, std::array<std::unique_ptr<CyclicInsertionPoint>, 2> InsertionPoints, GoalType GoalType);
		CyclicRule(const CyclicRule& other);
		CyclicRule(CyclicRule&& other) noexcept;
		~CyclicRule();
		CyclicRule& operator=(const CyclicRule& Other);
		CyclicRule& operator=(CyclicRule&& other) noexcept;

		void ReverseInsertionPoints();
		bool HasGoalType(GoalType GoalTypeToCheck) const;
		bool HasArcType(ArcType ArcTypeToCheck);
		GoalType GetGoalType() const;
		std::string GetGoalTypeToString() const;
		std::string GetName() const;
		ArcType GetArcType(int InsertionIndex);
		LevelElement::Element& GetElement(int InsertionIndex) const;

	private:
		std::string RuleName;
		GoalType GoalType;
		std::optional<std::array<std::unique_ptr<CyclicInsertionPoint>, 2>> InsertionPoints;

		friend class CyclicRuleRepository;
	};
}
