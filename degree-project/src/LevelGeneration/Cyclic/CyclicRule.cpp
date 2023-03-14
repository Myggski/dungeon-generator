#include "CyclicRule.h"

#include <algorithm>
#include <utility>

namespace Cyclic
{
	CyclicRule::CyclicRule(std::string RuleName, std::array<std::unique_ptr<CyclicInsertionPoint>, 2> InsertionPoints, enum GoalType GoalType)
		: RuleName(std::move(RuleName)),
		GoalType(GoalType),
		InsertionPoints(std::make_optional(std::move(InsertionPoints))) { }

	CyclicRule::CyclicRule(const CyclicRule& other)
		: RuleName(other.RuleName),
		GoalType(other.GoalType),
		InsertionPoints(std::nullopt)
	{
		if (other.InsertionPoints.has_value()) {
			InsertionPoints = std::array<std::unique_ptr<CyclicInsertionPoint>, 2>{
				std::make_unique<CyclicInsertionPoint>(*other.InsertionPoints.value()[0]),
					std::make_unique<CyclicInsertionPoint>(*other.InsertionPoints.value()[1])
			};
		}
	}

	CyclicRule::CyclicRule(CyclicRule&& other) noexcept
		: RuleName(std::move(other.RuleName)),
		GoalType(other.GoalType),
		InsertionPoints(std::move(other.InsertionPoints)) { }

	CyclicRule::~CyclicRule() = default;

	CyclicRule& CyclicRule::operator=(const CyclicRule& other)
	{
		if (this == &other) {
			return *this;
		}

		RuleName = other.RuleName;
		GoalType = other.GoalType;
		InsertionPoints = std::nullopt;

		if (other.InsertionPoints.has_value()) {
			InsertionPoints = std::array<std::unique_ptr<CyclicInsertionPoint>, 2>{
				std::make_unique<CyclicInsertionPoint>(*other.InsertionPoints.value()[0]),
					std::make_unique<CyclicInsertionPoint>(*other.InsertionPoints.value()[1])
			};
		}

		return *this;
	}

	CyclicRule& CyclicRule::operator=(CyclicRule&& other) noexcept
	{
		RuleName = std::move(other.RuleName);
		GoalType = other.GoalType;
		InsertionPoints = std::move(other.InsertionPoints);
		return *this;
	}

	bool CyclicRule::HasGoalType(Cyclic::GoalType GoalTypeToCheck) const
	{
		return (GoalType & GoalTypeToCheck) == GoalTypeToCheck;
	}
}