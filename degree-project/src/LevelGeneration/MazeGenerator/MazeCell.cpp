#include "MazeCell.h"

namespace MazeGenerator
{
	MazeCell::MazeCell(SDL_Point Position)
		: Position(Position),
		bIsRoom(false),
		bHasBeenVisited(false) { }

	void MazeCell::SetToRoom()
	{
		bIsRoom = true;
	}

	void MazeCell::SetToWall()
	{
		bIsRoom = false;
	}

	void MazeCell::SetToVisited()
	{
		bHasBeenVisited = true;
	}

	bool MazeCell::IsRoom() const
	{
		return bIsRoom;
	}

	bool MazeCell::IsVisited() const
	{
		return bHasBeenVisited;
	}
}