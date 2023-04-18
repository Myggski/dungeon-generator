#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <SDL.h>

#include "LevelGeneration/LevelElement/Element.h"
#include "LevelGeneration/LevelGenerator/RuleLevel/DirectionType.h"
#include "LevelGeneration/LevelGenerator/RoomType.h"

namespace Application
{
	class Renderer;
}

namespace Command
{
	class CreateHiResLevelCommand;
}

namespace LevelGeneration
{
	class Level;

	class Room
	{
	public:
		Room();
		Room(SDL_FRect RoomRect);
		bool operator==(const Room& Other) const;

		void AdjustWidth(float GridWidthToAdjust, float MoveX = 0.f);
		void AdjustHeight(float GridHeightToAdjust, float MoveY = 0.f);
		void Draw(Application::Renderer& Renderer, const std::tuple<int, int>& CellSize, SDL_Color Color = { 199, 220, 208, 255 });

	private:
		void SetEntrance(DirectionType Entrance);
		bool HasEntrance(DirectionType EntranceDirection) const;
		LevelGenerator::RoomType GetType() const;
		void SetType(LevelGenerator::RoomType Type);

	private:
		size_t RoomId;
		inline static size_t NextRoomId{ 0 };
		
		LevelGenerator::RoomType Type;
		DirectionType EntranceFlag;
		SDL_FRect RoomRect;
		// Spatial Hash | Elements
		std::unordered_map<std::string, std::vector<std::shared_ptr<LevelElement::Element>>> Elements;

		friend class LevelGeneration::Level;
		friend class Command::CreateHiResLevelCommand;
	};
}
