#pragma once
#include <SDL_rect.h>

namespace MazeGenerator
{
	class Maze;

	class MazeCell
	{
	public:
		MazeCell(SDL_Point Position);

		inline SDL_Point GetPosition() const
		{
			return Position;
		}

		void SetToWall();
		void SetToRoom();
		void SetVisited(bool bVisited);
		bool IsRoom() const;
		bool IsVisited() const;

	private:
		SDL_Point Position;
		bool bIsRoom;
		bool bHasBeenVisited;
	};
}
