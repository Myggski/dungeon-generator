#include "LowResCell.h"

namespace LevelGenerator
{
	LowResCell::LowResCell()
		: Position(std::make_tuple(std::numeric_limits<int>::min(), std::numeric_limits<int>::min())),
		Type(LowResCellType::None) {}

	LowResCell::LowResCell(std::tuple<int, int> Position, LowResCellType Type, std::vector<std::shared_ptr<LevelElement::Element>> Elements)
		: Position(Position),
		Type(Type),
		Elements(std::move(Elements)) {}

	std::tuple<int, int> LowResCell::GetPosition() const
	{
		return Position;
	}

	const std::vector<std::shared_ptr<LevelElement::Element>>& LowResCell::GetElements() const
	{
		return Elements;
	}

	LowResCellType LowResCell::GetType() const
	{
		return Type;
	}
}