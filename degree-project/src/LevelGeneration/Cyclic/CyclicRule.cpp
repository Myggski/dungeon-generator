#include "CyclicRule.h"

#include <algorithm>
#include <stdexcept>
#include <utility>
#include "LevelGeneration/LevelElement/KillTargetProperty.h"

namespace Cyclic
{

	CyclicRule::CyclicRule(std::string RuleName, std::array<std::unique_ptr<CyclicInsertionPoint>, 2> InsertionPoints, LevelElement::Element GoalElement)
		: RuleName(std::move(RuleName)),
		GoalElement(GoalElement),
		InsertionPoints(std::make_optional(std::move(InsertionPoints))) { }

	CyclicRule::CyclicRule(const CyclicRule& other)
		: RuleName(other.RuleName),
		GoalElement(other.GoalElement),
		InsertionPoints(std::nullopt)
	{
		if (other.InsertionPoints.has_value()) {
			InsertionPoints = std::array
			{
				std::make_unique<CyclicInsertionPoint>(*other.InsertionPoints.value()[0]),
					std::make_unique<CyclicInsertionPoint>(*other.InsertionPoints.value()[1])
			};
		}
	}

	CyclicRule::CyclicRule(CyclicRule&& Other) noexcept
		: RuleName(std::move(Other.RuleName)),
		GoalElement(std::move(Other.GoalElement)),
		InsertionPoints(std::move(Other.InsertionPoints)) { }

	CyclicRule::~CyclicRule() = default;

	CyclicRule& CyclicRule::operator=(const CyclicRule& Other)
	{
		if (this == &Other) {
			return *this;
		}

		RuleName = Other.RuleName;
		GoalElement = Other.GoalElement;
		InsertionPoints = std::nullopt;

		if (Other.InsertionPoints.has_value()) {
			InsertionPoints = std::array<std::unique_ptr<CyclicInsertionPoint>, 2>{
				std::make_unique<CyclicInsertionPoint>(*Other.InsertionPoints.value()[0]),
					std::make_unique<CyclicInsertionPoint>(*Other.InsertionPoints.value()[1])
			};
		}

		return *this;
	}

	CyclicRule& CyclicRule::operator=(CyclicRule&& Other) noexcept
	{
		RuleName = std::move(Other.RuleName);
		GoalElement = std::move(Other.GoalElement);
		InsertionPoints = std::move(Other.InsertionPoints);
		return *this;
	}

	/**
	 * \brief Is used to make sure that a short path is being created first
	 */
	void CyclicRule::ReverseInsertionPoints()
	{
		if (InsertionPoints.has_value())
		{
			std::ranges::reverse(InsertionPoints.value());
		}
	}

	bool CyclicRule::HasArcType(ArcType ArcTypeToCheck)
	{
		if (!InsertionPoints.has_value())
		{
			return false;
		}

		return std::ranges::any_of(InsertionPoints.value(), [&](const auto& InsertionPoint) { return InsertionPoint->ArcType == ArcTypeToCheck; });
	}

	ArcType CyclicRule::GetArcType(int InsertionIndex) const
	{
		if (!InsertionPoints.has_value() || InsertionIndex > InsertionPoints.value().size() - 1)
		{
			return ArcType::None;
		}

		return InsertionPoints.value()[InsertionIndex]->ArcType;
	}

	std::string CyclicRule::GetName() const
	{
		return RuleName;
	}
	
	LevelElement::Element& CyclicRule::GetElement(int InsertionIndex) const
	{
		if (!InsertionPoints.has_value() || InsertionIndex > InsertionPoints.value().size() - 1)
		{
			throw std::runtime_error("Something went wrong when trying to get an Element");
		}

		return InsertionPoints.value()[InsertionIndex]->GetElement();
	}

	const LevelElement::Element& CyclicRule::GetGoalElement() const
	{
		return GoalElement;
	}
}
