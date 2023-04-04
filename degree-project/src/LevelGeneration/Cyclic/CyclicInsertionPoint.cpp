#include "CyclicInsertionPoint.h"
#include "CyclicRule.h"
#include "LevelGeneration/LevelElement/LockKeyProperty.h"
#include "LevelGeneration/LevelElement/MiniBossProperty.h"
#include "LevelGeneration/LevelElement/RewardProperty.h"

namespace Cyclic
{
	CyclicInsertionPoint::CyclicInsertionPoint(const LevelElement::Element&& LevelElement, Cyclic::ArcType ArcType)
		: ArcType(ArcType),
		LevelElement(LevelElement),
		MinorRule(nullptr) { }

	CyclicInsertionPoint::CyclicInsertionPoint(const CyclicInsertionPoint& Other)
		: ArcType(Other.ArcType),
		LevelElement(Other.LevelElement),
		MinorRule(Other.MinorRule.get()) { }

	void CyclicInsertionPoint::AddMinorRule(std::unique_ptr<CyclicRule> Rule)
	{
		MinorRule = std::move(Rule);
	}

	bool CyclicInsertionPoint::HasMinorRule() const
	{
		return MinorRule != nullptr;
	}

	LevelElement::Element& CyclicInsertionPoint::GetElement()
	{
		return LevelElement;
	}
}
