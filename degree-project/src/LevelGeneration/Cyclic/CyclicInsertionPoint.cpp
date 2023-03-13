#include "CyclicInsertionPoint.h"

#include "CyclicRule.h"

namespace Cyclic
{

	CyclicInsertionPoint::CyclicInsertionPoint(Cyclic::ArcType ArcType, Cyclic::InsertionType InsertionType)
		: ArcType(ArcType),
		  InsertionType(InsertionType),
		  MinorRule(std::nullopt) { }

	void CyclicInsertionPoint::AddMinorRule(CyclicRule& Rule)
	{
		MinorRule = std::make_optional(Rule);
	}

	bool CyclicInsertionPoint::HasMinorRule() const
	{
		return MinorRule.has_value();
	}
}
