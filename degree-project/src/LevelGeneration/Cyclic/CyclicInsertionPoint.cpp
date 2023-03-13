#include "CyclicInsertionPoint.h"

#include "CyclicRule.h"

namespace Cyclic
{

	CyclicInsertionPoint::CyclicInsertionPoint(Cyclic::ArcType ArcType, Cyclic::InsertionType InsertionType)
		: ArcType(ArcType),
		  InsertionType(InsertionType),
		  MinorRule(std::nullopt) { }

	void CyclicInsertionPoint::AddMinorRule(std::optional<CyclicRule> Rule)
	{
		MinorRule = std::move(Rule);
	}

	bool CyclicInsertionPoint::HasMinorRule() const
	{
		return MinorRule.has_value();
	}
}
