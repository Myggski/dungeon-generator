#include "CyclicRule.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace Cyclic
{
	CyclicRule::CyclicRule(std::string RuleName, std::array<std::unique_ptr<CyclicInsertionPoint>, 2> InsertionPoints, enum GoalType GoalType)
		: RuleName(std::move(RuleName)),
		GoalType(GoalType),
		InsertionPoints(std::make_optional(std::move(InsertionPoints))) { }

	CyclicRule::CyclicRule(const CyclicRule& other)
		: RuleName(other.RuleName),
		GoalType(other.GoalType),
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

	CyclicRule::CyclicRule(CyclicRule&& other) noexcept
		: RuleName(std::move(other.RuleName)),
		GoalType(other.GoalType),
		InsertionPoints(std::move(other.InsertionPoints)) { }

	CyclicRule::~CyclicRule() = default;

	CyclicRule& CyclicRule::operator=(const CyclicRule& Other)
	{
		if (this == &Other) {
			return *this;
		}

		RuleName = Other.RuleName;
		GoalType = Other.GoalType;
		InsertionPoints = std::nullopt;

		if (Other.InsertionPoints.has_value()) {
			InsertionPoints = std::array<std::unique_ptr<CyclicInsertionPoint>, 2>{
				std::make_unique<CyclicInsertionPoint>(*Other.InsertionPoints.value()[0]),
					std::make_unique<CyclicInsertionPoint>(*Other.InsertionPoints.value()[1])
			};
		}

		return *this;
	}

	CyclicRule& CyclicRule::operator=(CyclicRule&& other) noexcept
	{
		RuleName = std::move(other.RuleName);
		GoalType = other.GoalType;
		InsertionPoints = std::move(other.InsertionPoints);
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

	bool CyclicRule::HasGoalType(Cyclic::GoalType GoalTypeToCheck) const
	{
		return (GoalType & GoalTypeToCheck) == GoalTypeToCheck;
	}

	bool CyclicRule::HasArcType(ArcType ArcTypeToCheck)
	{
		if (!InsertionPoints.has_value())
		{
			return false;
		}

		return std::ranges::any_of(InsertionPoints.value(), [&](const auto& InsertionPoint) { return InsertionPoint->ArcType == ArcTypeToCheck; });
	}

	GoalType CyclicRule::GetGoalType() const
	{
		return GoalType;
	}

	std::string CyclicRule::GetGoalTypeToString() const
	{
		switch (GoalType)
		{
		case GoalType::Treasure:
			return "Treasure";
		case GoalType::SecretDocuments:
			return "Secret Document";
		case GoalType::KillTarget:
		default:
			return "Kill Target";
		}
	}

	ArcType CyclicRule::GetArcType(int InsertionIndex)
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
}
