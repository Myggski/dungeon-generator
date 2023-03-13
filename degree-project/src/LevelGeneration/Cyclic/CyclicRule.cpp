#include "CyclicRule.h"

#include <utility>

namespace Cyclic
{
	CyclicRule::CyclicRule(std::string RuleName, std::array<std::unique_ptr<CyclicInsertionPoint>, 2> InsertionPoints, enum GoalType GoalType)
		: RuleName(std::move(RuleName)),
		GoalType(GoalType),
		InsertionPoints(std::move(InsertionPoints)) { }
}