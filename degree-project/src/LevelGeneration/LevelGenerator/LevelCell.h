#pragma once

#include <SDL_rect.h>
#include <vector>

#include "DirectionType.h"
#include "LevelGeneration/LevelElement/Element.h"

namespace LevelGenerator
{
	class LevelCell
	{
	public:
		LevelCell();
		LevelCell(SDL_Point Position);
		bool operator==(const LevelCell& Other) const
		{
			return Position.x == Other.Position.x
				&& Position.y == Other.Position.y;
		}

		SDL_Point GetPosition() const
		{
			return Position;
		}

		void CarveEntrance(DirectionType Direction);
		void CollapseEntrance(DirectionType Direction);
		void SetVisited(bool bVisited);
		void AddElement(LevelElement::Element Element);
		bool IsVisited() const;
		DirectionType GetEntranceFlag() const;
		const std::vector<LevelElement::Element>& GetElements() const;

	private:
		SDL_Point Position;
		std::vector<LevelElement::Element> Elements;
		// Flag for where the entrance(s) are (North, East, South and/or West)
		DirectionType EntranceFlag;
		bool bHasBeenVisited;
	};
}
