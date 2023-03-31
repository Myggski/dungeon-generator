#include "CyclicRuleRepository.h"

#include <algorithm>
#include "utils/RandomGenerator.h"

namespace Cyclic
{
	CyclicRuleRepository::CyclicRuleRepository(int NumberOfRules)
	{
		ListOfRules.reserve(NumberOfRules);
	}

	void CyclicRuleRepository::Add(CyclicRule&& Rule)
	{
		ListOfRules.emplace_back(std::move(Rule));
	}

	void CyclicRuleRepository::Remove(const CyclicRule& RuleToRemove)
	{
		const auto RemovableIterator = std::ranges::remove_if(ListOfRules,[&RuleToRemove](const CyclicRule& Rule) { return Rule.RuleName == RuleToRemove.RuleName; }).begin();

		if (RemovableIterator != ListOfRules.end())
		{
			ListOfRules.erase(RemovableIterator, ListOfRules.end());
		}
	}

	CyclicRule& CyclicRuleRepository::GetRandomRule()
	{
		const int RandomIndex = Utils::RandomGenerator::GetInstance().GetRandom(0, static_cast<int>(ListOfRules.size() - 1));

		return ListOfRules[RandomIndex];
	}
}
