#include "MazeCell.h"

namespace MazeGenerator
{
	MazeCell::MazeCell()
		: Position({ -1, -1 }),
		  EntranceFlag(DirectionType::None),
	      bHasBeenVisited(false) { }

	MazeCell::MazeCell(SDL_Point Position)
		: Position(Position),
	      EntranceFlag(DirectionType::None),
		  bHasBeenVisited(false) { }

	void MazeCell::CarveEntrance(DirectionType Direction)
	{
		EntranceFlag = (EntranceFlag | Direction);
	}

	void MazeCell::SetVisited(bool bVisited)
	{
		bHasBeenVisited = bVisited;
	}

	bool MazeCell::IsVisited() const
	{
		return bHasBeenVisited;
	}

	DirectionType MazeCell::GetEntranceFlag() const
	{
		return EntranceFlag;
	}
}