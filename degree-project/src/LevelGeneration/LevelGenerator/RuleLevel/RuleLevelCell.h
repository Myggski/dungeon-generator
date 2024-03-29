#pragma once

#include <memory>
#include <SDL_rect.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "DirectionType.h"

namespace LevelElement
{
	struct Element;
}

namespace LevelGenerator
{
	using SpatialHash = std::string;

	struct CellEntranceData
	{
	public:
		int ConnectedPaths { 0 };
		DirectionType Direction { DirectionType::None };
	};

	class RuleLevelCell
	{
	public:
		RuleLevelCell();
		RuleLevelCell(SDL_Point Position);
		bool operator==(const RuleLevelCell& Other) const
		{
			return Position.x == Other.Position.x
				&& Position.y == Other.Position.y;
		}

		SDL_Point GetPosition() const
		{
			return Position;
		}

		void CarveEntrance(const std::string& ConnectedToSpatialHash);
		void CollapseEntrance(const SpatialHash& ConnectedTo);
		void SetVisited(bool bVisited);
		void AddElement(const LevelElement::Element& Element);
		void RemoveElement(int ElementIndex);
		bool IsVisited() const;
		DirectionType GetEntranceFlag() const;
		bool HasEntrance(DirectionType Direction) const;
		const std::vector<std::shared_ptr<LevelElement::Element>>& GetElements() const;
		int GetNumberOfEntrances() const;
		std::string GetSpatialHash() const;
		static std::tuple<int, int> SpatialHashToCoordinates(const SpatialHash& SpatialHash);

	private:
		DirectionType SpatialHashToDirectionType(const SpatialHash& SpatialHash) const;

	private:
		SDL_Point Position;
		std::vector<std::shared_ptr<LevelElement::Element>> Elements;
		std::vector<std::string> CarvedToSpatialHash;
		// Direction and number of paths that's crossing it
		std::unordered_map<DirectionType, int>  EntranceDirections;
		// Directions that has an entrance
		DirectionType EntranceFlag;
		bool bHasBeenVisited;
	};
}
