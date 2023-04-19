#include "RuleLevelCell.h"

#include <ranges>
#include <sstream>

#include "LevelGeneration/LevelElement/Element.h"

namespace LevelGenerator
{
	RuleLevelCell::RuleLevelCell()
		: Position({ -1, -1 }),
		EntranceDirections({}),
		EntranceFlag(DirectionType::None),
	    bHasBeenVisited(false) { }

	RuleLevelCell::RuleLevelCell(SDL_Point Position)
		: Position(Position),
		EntranceDirections({}),
		EntranceFlag(DirectionType::None),
		bHasBeenVisited(false) { }

	void RuleLevelCell::CarveEntrance(const SpatialHash& ConnectedTo)
	{
		const DirectionType Direction = SpatialHashToDirectionType(ConnectedTo);
		EntranceFlag = EntranceFlag | Direction;
		EntranceDirections[Direction]++;
	}

	void RuleLevelCell::CollapseEntrance(const SpatialHash& ConnectedTo)
	{
		const DirectionType Direction = SpatialHashToDirectionType(ConnectedTo);

		if (Direction == DirectionType::None)
		{
			return;
		}

		EntranceDirections[Direction]--;

		if (EntranceDirections[Direction] <= 0)
		{
			EntranceFlag = EntranceFlag & ~Direction;
			EntranceDirections.erase(Direction);
		}
	}

	void RuleLevelCell::SetVisited(bool bVisited)
	{
		bHasBeenVisited = bVisited;
	}

	void RuleLevelCell::AddElement(const LevelElement::Element& Element)
	{
		Elements.push_back(std::make_shared<LevelElement::Element>(Element));
	}

	void RuleLevelCell::RemoveElement(int ElementIndex)
	{
		Elements.erase(Elements.begin() + ElementIndex);
	}

	bool RuleLevelCell::IsVisited() const
	{
		return bHasBeenVisited;
	}

	const std::vector<std::shared_ptr<LevelElement::Element>>& RuleLevelCell::GetElements() const
	{
		return Elements;
	}

	std::string RuleLevelCell::GetSpatialHash() const
	{
		std::stringstream SpatialHashStream;
		SpatialHashStream << Position.x << "." << Position.y;
		return SpatialHashStream.str();
	}

	std::tuple<int, int> RuleLevelCell::SpatialHashToCoordinates(const SpatialHash& SpatialHash)
	{
		int x, y;
		std::stringstream ss(SpatialHash);

		ss >> x;
		ss.ignore(1, '.');
		ss >> y;

		return std::make_tuple(x, y);
	}

	int RuleLevelCell::GetNumberOfEntrances() const
	{
		return static_cast<int>(EntranceDirections.size());
	}

	DirectionType RuleLevelCell::GetEntranceFlag() const
	{
		return EntranceFlag;
	}

	bool RuleLevelCell::HasEntrance(DirectionType Direction) const
	{
		return static_cast<bool>((EntranceFlag & Direction));
	}

	DirectionType RuleLevelCell::SpatialHashToDirectionType(const SpatialHash& SpatialHash) const
	{
		auto [PositionX, PositionY] = SpatialHashToCoordinates(SpatialHash);

		DirectionType CarvedDirection = DirectionType::None;
		const int DirectionX = PositionX - Position.x;
		const int DirectionY = PositionY - Position.y;

		if (DirectionX > 0)
		{
			CarvedDirection = DirectionType::East;
		}
		else if (DirectionX < 0)
		{
			CarvedDirection = DirectionType::West;
		}

		if (DirectionY > 0)
		{
			CarvedDirection = DirectionType::South;
		}
		else if (DirectionY < 0)
		{
			CarvedDirection = DirectionType::North;
		}

		return CarvedDirection;
	}

}
