#pragma once

#include <vector>

namespace Cyclic
{
	class CyclicRule;

	class CyclicRuleRepository
	{
	public:
		CyclicRuleRepository(int NumberOfRules);

		void Add(const CyclicRule&& Rule);
		void Remove(const CyclicRule& RuleName);
		const CyclicRule& GetRandomRule();

	private:
		std::vector<CyclicRule> ListOfRules;

	};
}
