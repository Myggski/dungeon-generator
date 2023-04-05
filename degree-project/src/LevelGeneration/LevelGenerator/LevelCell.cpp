#include "LevelCell.h"

#include <ranges>
#include <sstream>

#include "LevelGeneration/LevelElement/Element.h"

namespace LevelGenerator
{
	LevelCell::LevelCell()
		: Position({ -1, -1 }),
		EntranceDirections({}),
		EntranceFlag(DirectionType::None),
	    bHasBeenVisited(false) { }

	LevelCell::LevelCell(SDL_Point Position)
		: Position(Position),
		EntranceDirections({}),
		EntranceFlag(DirectionType::None),
		bHasBeenVisited(false) { }

	void LevelCell::CarveEntrance(const SpatialHash& ConnectedTo)
	{
		const DirectionType Direction = SpatialHashToDirectionType(ConnectedTo);
		EntranceFlag = EntranceFlag | Direction;
		EntranceDirections[Direction]++;
	}

	void LevelCell::CollapseEntrance(const SpatialHash& ConnectedTo)
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

	void LevelCell::SetVisited(bool bVisited)
	{
		bHasBeenVisited = bVisited;
	}

	void LevelCell::AddElement(LevelElement::Element& Element)
	{
		Elements.push_back(std::make_shared<LevelElement::Element>(Element));
	}

	void LevelCell::RemoveElement(int ElementIndex)
	{
		Elements.erase(Elements.begin() + ElementIndex);
	}

	bool LevelCell::IsVisited() const
	{
		return bHasBeenVisited;
	}

	const std::vector<std::shared_ptr<LevelElement::Element>>& LevelCell::GetElements() const
	{
		return Elements;
	}

	std::string LevelCell::GetSpatialHash() const
	{
		std::stringstream SpatialHashStream;
		SpatialHashStream << Position.x << "." << Position.y;
		return SpatialHashStream.str();
	}

	std::tuple<int, int> LevelCell::SpatialHashToCoordinates(const SpatialHash& SpatialHash)
	{
		int x, y;
		std::stringstream ss(SpatialHash);

		ss >> x;
		ss.ignore(1, '.');
		ss >> y;

		return std::make_tuple(x, y);
	}

	int LevelCell::GetNumberOfEntrances() const
	{
		return static_cast<int>(EntranceDirections.size());
	}

	
	DirectionType LevelCell::GetEntranceFlag() const
	{
		return EntranceFlag;
	}

	DirectionType LevelCell::SpatialHashToDirectionType(const SpatialHash& SpatialHash) const
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
