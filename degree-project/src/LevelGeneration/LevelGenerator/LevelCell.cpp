#include "LevelCell.h"

namespace LevelGenerator
{
	LevelCell::LevelCell()
		: Position({ -1, -1 }),
		  EntranceFlag(DirectionType::None),
	      bHasBeenVisited(false) { }

	LevelCell::LevelCell(SDL_Point Position)
		: Position(Position),
	      EntranceFlag(DirectionType::None),
		  bHasBeenVisited(false) { }

	void LevelCell::CarveEntrance(DirectionType Direction)
	{
		EntranceFlag = (EntranceFlag | Direction);
	}

	void LevelCell::CollapseEntrance(DirectionType Direction)
	{
		EntranceFlag = (EntranceFlag & ~Direction);
	}

	void LevelCell::SetVisited(bool bVisited)
	{
		bHasBeenVisited = bVisited;
	}

	void LevelCell::AddElement(LevelElement::Element Element)
	{
		Elements.push_back(std::move(Element));
	}

	bool LevelCell::IsVisited() const
	{
		return bHasBeenVisited;
	}

	const std::vector<LevelElement::Element>& LevelCell::GetElements() const
	{
		return Elements;
	}
	
	DirectionType LevelCell::GetEntranceFlag() const
	{
		return EntranceFlag;
	}
}