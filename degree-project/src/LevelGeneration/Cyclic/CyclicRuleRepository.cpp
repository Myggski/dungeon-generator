#include "CyclicRuleRepository.h"

#include <algorithm>

#include "CyclicRule.h"
#include "utils/RandomGenerator.h"

namespace Cyclic
{
	CyclicRuleRepository::CyclicRuleRepository(int NumberOfRules)
	{
		ListOfRules.reserve(NumberOfRules);
	}

	void CyclicRuleRepository::Add(const CyclicRule&& Rule)
	{
		ListOfRules.emplace_back(Rule);
	}

	void CyclicRuleRepository::Remove(const CyclicRule& RuleToRemove)
	{
		std::erase_if(ListOfRules,[RuleToRemove](CyclicRule const& Rule) { return Rule.RuleName == RuleToRemove.RuleName; });
	}

	const CyclicRule& CyclicRuleRepository::GetRandomRule()
	{
		return ListOfRules[Utils::RandomGenerator::GetInstance().GetRandom<int>(0, static_cast<int>(ListOfRules.size() - 1))];
	}
}
