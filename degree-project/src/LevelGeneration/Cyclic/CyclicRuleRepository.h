#pragma once

#include <vector>
#include "CyclicRule.h"

namespace Cyclic
{
	class CyclicRuleRepository
	{
	public:
		CyclicRuleRepository(int NumberOfRules);

		void Add(CyclicRule&& Rule);
		void Remove(const CyclicRule& RuleName);
		CyclicRule& GetRandomRule();

	private:
		std::vector<CyclicRule> ListOfRules;

	};
}
