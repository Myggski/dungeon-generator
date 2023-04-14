#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <SDL.h>

#include "LevelGeneration/LevelElement/Element.h"

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

	enum class RoomType : uint16_t
	{
		None = 0,
		Cargo = 1 << 1,
		CrewQuarters = 1 << 2,
		MedicalBay = 1 << 3,
		Bridge = 1 << 4,
		ScienceLab = 1 << 5,
		Communication = 1 << 6,
		Equipment = 1 << 7,
		Laundry = 1 << 8,
		Exercise = 1 << 9,
		EscapePods = 1 << 10,
		Workshop = 1 << 11,
		Hub = 1 << 12
	};

	class Room
	{
	public:
		Room();
		Room(SDL_FRect RoomRect);
		bool operator==(const Room& Other) const;

		void Draw(Application::Renderer& Renderer, const std::tuple<int, int>& CellSize, SDL_Color Color = { 199, 220, 208, 255 });

	private:
		size_t RoomId;
		inline static size_t NextRoomId{ 0 };
		
		RoomType Type;
		SDL_FRect RoomRect;
		// Spatial Hash | Elements
		std::unordered_map<std::string, std::vector<LevelElement::Element>> Elements;

		friend class LevelGeneration::Level;
		friend class Command::CreateHiResLevelCommand;
	};
}
