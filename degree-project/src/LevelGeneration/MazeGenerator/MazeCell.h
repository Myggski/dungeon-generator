#pragma once
#include <SDL_rect.h>
#include "DirectionType.h"

namespace MazeGenerator
{
	class MazeCell
	{
	public:
		MazeCell();
		MazeCell(SDL_Point Position);
		bool operator==(const MazeCell& other)
		{
			return Position.x == other.Position.x
				&& Position.y == other.Position.y;
		}

		inline SDL_Point GetPosition() const
		{
			return Position;
		}

		void CarveEntrance(DirectionType Direction);
		void SetVisited(bool bVisited);
		bool IsVisited() const;
		DirectionType GetEntranceFlag() const;

	private:
		SDL_Point Position;
		// Flag for where the entrance(s) are (North, East, South and/or West)
		DirectionType EntranceFlag;
		bool bHasBeenVisited;
	};
}
