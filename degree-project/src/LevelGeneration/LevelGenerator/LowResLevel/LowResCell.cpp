#include "LowResCell.h"

namespace LevelGenerator
{
	LowResCell::LowResCell()
		: Position(std::make_tuple(std::numeric_limits<int>::min(), std::numeric_limits<int>::min())),
		Type(LowResCellType::None),
		EntranceFlag(DirectionType::None) {}

	LowResCell::LowResCell(std::tuple<int, int> Position, LowResCellType Type, std::vector<std::shared_ptr<LevelElement::Element>> Elements)
		: Position(Position),
		Type(Type),
		Elements(std::move(Elements)),
		EntranceFlag(DirectionType::None) {}

	DirectionType LowResCell::GetEntrance() const
	{
		return EntranceFlag;
	}

	void LowResCell::SetEntrance(DirectionType EntranceFlag)
	{
		this->EntranceFlag = EntranceFlag;
	}

	void LowResCell::SetRoomType(LevelGenerator::RoomType RoomType)
	{
		this->RoomType = RoomType;
	}

	void LowResCell::SetNumberOfEntrances(int NumberOfEntrances)
	{
		this->NumberOfEntrances = NumberOfEntrances;
	}

	int LowResCell::GetNumberOfEntrances()
	{
		return NumberOfEntrances;
	}

	std::tuple<int, int> LowResCell::GetPosition() const
	{
		return Position;
	}

	const std::vector<std::shared_ptr<LevelElement::Element>>& LowResCell::GetElements() const
	{
		return Elements;
	}

	RoomType LowResCell::GetRoomType() const
	{
		return RoomType;
	}

	LowResCellType LowResCell::GetType() const
	{
		return Type;
	}
}