#pragma once

#include <cstdint>
#include <memory>

#include "LevelGeneration/LevelElement/Element.h"

namespace Cyclic
{
	class CyclicRule;

	enum class ArcType : uint8_t
	{
		None = 0,
		Short = 1 << 1,
		Long = 1 << 2,
	};

	class CyclicInsertionPoint
	{
	public:
		CyclicInsertionPoint(const LevelElement::Element&& LevelElement, ArcType ArcType = ArcType::None);
		CyclicInsertionPoint(const CyclicInsertionPoint& Other);

		void AddMinorRule(std::unique_ptr<CyclicRule> Rule);
		LevelElement::Element& GetElement();
		bool HasMinorRule() const;

	private:
		ArcType ArcType;
		LevelElement::Element LevelElement;
		std::unique_ptr<CyclicRule> MinorRule;

		friend class CyclicRule;
	};
}
