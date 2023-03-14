#include "CyclicInsertionPoint.h"
#include "CyclicRule.h"

namespace Cyclic
{
	

	CyclicInsertionPoint::CyclicInsertionPoint(Cyclic::ArcType ArcType, Cyclic::InsertionType InsertionType)
		: ArcType(ArcType),
		  InsertionType(InsertionType),
		  MinorRule(nullptr) { }

	CyclicInsertionPoint::CyclicInsertionPoint(const CyclicInsertionPoint& other)
		: ArcType(other.ArcType),
		InsertionType(other.InsertionType)
	{
		if (other.MinorRule)
		{
			MinorRule = std::make_unique<CyclicRule>(*other.MinorRule);
		}
	}


	void CyclicInsertionPoint::AddMinorRule(std::unique_ptr<CyclicRule> Rule)
	{
		MinorRule = std::move(Rule);
	}

	bool CyclicInsertionPoint::HasMinorRule() const
	{
		return MinorRule != nullptr;
	}
}
