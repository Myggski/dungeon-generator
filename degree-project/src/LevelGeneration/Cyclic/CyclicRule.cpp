#include "CyclicRule.h"

#include <utility>

namespace Cyclic
{
	CyclicRule::CyclicRule(std::string RuleName, std::array<CyclicInsertionPoint&, 2> InsertionPoints, enum GoalType GoalType)
		: RuleName(std::move(RuleName)),
		GoalType(GoalType),
		InsertionPoints(InsertionPoints) { }
}