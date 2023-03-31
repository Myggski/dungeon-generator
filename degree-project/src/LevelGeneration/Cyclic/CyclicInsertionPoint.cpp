#include "CyclicInsertionPoint.h"
#include "CyclicRule.h"
#include "LevelGeneration/LevelElement/LockKeyProperty.h"
#include "LevelGeneration/LevelElement/MiniBossProperty.h"
#include "LevelGeneration/LevelElement/RewardProperty.h"

namespace Cyclic
{
	CyclicInsertionPoint::CyclicInsertionPoint(const LevelElement::Element& LevelElement, Cyclic::ArcType ArcType)
		: ArcType(ArcType),
		ElementType(GetElementType(LevelElement.GetProperty())),
		LevelElement(LevelElement),
		MinorRule(nullptr) { }

	CyclicInsertionPoint::CyclicInsertionPoint(const CyclicInsertionPoint& Other)
		: ArcType(Other.ArcType),
		ElementType(GetElementType(Other.LevelElement.GetProperty())),
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

	LevelElement::Element CyclicInsertionPoint::GetElement()
	{
		return LevelElement;
	}

	ElementType CyclicInsertionPoint::GetElementType(LevelElement::ElementProperty& Property) const
	{
		if (dynamic_cast<LevelElement::RewardProperty*>(&Property))
		{
			return ElementType::Reward;
		}

		if (dynamic_cast<LevelElement::MiniBossProperty*>(&Property))
		{
			return ElementType::MiniBoss;
		}

		if (dynamic_cast<LevelElement::LockKeyProperty*>(&Property))
		{
			return ElementType::LockAndKey;
		}

		return ElementType::None;
	}
}
