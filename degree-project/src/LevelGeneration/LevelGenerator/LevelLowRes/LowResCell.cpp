#include "LowResCell.h"

namespace LevelGenerator
{
	LowResCell::LowResCell()
		: Position(std::make_tuple(std::numeric_limits<int>::min(), std::numeric_limits<int>::min())),
		Type(LowResCellType::None) {}

	LowResCell::LowResCell(std::tuple<int, int> Position, LowResCellType Type) 
		: Position(Position),
		Type(Type) {}

	std::tuple<int, int> LowResCell::GetPosition() const
	{
		return Position;
	}

	LowResCellType LowResCell::GetType() const
	{
		return Type;
	}
}