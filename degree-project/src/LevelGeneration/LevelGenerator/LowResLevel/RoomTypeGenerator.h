#pragma once

#include <unordered_map>
#include <array>
#include <vector>
#include <ranges>
#include <tuple>
#include <span>

#include "LevelGeneration/LevelGenerator/RoomType.h"
#include "LevelGeneration/LevelGenerator/LowResLevel/LowResCell.h"
#include "LevelGeneration/LevelGenerator/LowResLevel/LowResLevelStateData.h"
#include <Utils/RandomGenerator.h>

namespace LevelGenerator
{
	namespace RoomTypeGenerator
	{
		static const std::unordered_map<RoomType, int> MaxNumberOfRoomsRule 
		{
			{ RoomType::Cargo, 3 },
			{ RoomType::CrewQuarters, 3 },
			{ RoomType::MedicalBay, 2 },
			{ RoomType::Bridge, 1 },
			{ RoomType::ScienceLab, 1 },
			{ RoomType::Communication, 1 },
			{ RoomType::Equipment, 2 },
			{ RoomType::Laundry, 1 },
			{ RoomType::Exercise, 1 },
			{ RoomType::EscapePods, 1 },
			{ RoomType::Workshop, 1 },
			{ RoomType::HoldingBay, 1 },
			{ RoomType::Hub, 2 },
		};

		static const std::unordered_map<RoomType, int> NumberOfEntrancesRule
		{
			{ RoomType::HoldingBay, 1 },
			{ RoomType::Hub, 3 },
		};

		static const std::unordered_map<std::string, RoomType> ExactPositionRule
		{
			{ "2.0", RoomType::Bridge },
		};

		static const std::unordered_map<RoomType, DirectionType> DirectionRule 
		{
			{ RoomType::Cargo, DirectionType::South },
			{ RoomType::Communication, DirectionType::North },
			{ RoomType::ScienceLab, DirectionType::North },
			{ RoomType::EscapePods, DirectionType::South },
			{ RoomType::Laundry, DirectionType::South },
		};

		constexpr RoomType Rooms[] = 
		{ 
			RoomType::Cargo, 
			RoomType::CrewQuarters, 
			RoomType::MedicalBay, 
			RoomType::Bridge, 
			RoomType::ScienceLab, 
			RoomType::Communication, 
			RoomType::Equipment, 
			RoomType::Laundry, 
			RoomType::Exercise, 
			RoomType::EscapePods, 
			RoomType::Workshop, 
			RoomType::HoldingBay,
			RoomType::Hub 
		};

		RoomType GetRoomType(int X, int Y, LowResLevelStateData& LowResLevelStateData, const std::unordered_map<RoomType, int>& AlreadySelectedRooms)
		{
			std::vector<RoomType> AvailableRooms;
			std::ranges::copy(Rooms, std::back_inserter(AvailableRooms));
			LowResCell* CurrentCell = LowResLevelStateData.GetCell(X * 2, Y * 2);

			std::string SpatialKey = std::to_string(X) + "." + std::to_string(Y);
			// If exact position exists, return the RoomType
			if (ExactPositionRule.contains(SpatialKey))
			{
				return ExactPositionRule.at(SpatialKey);
			}
			else 
			{
				// Remove the room if it's not in the exact position
				std::erase_if(AvailableRooms, [&](RoomType Room) {
					for (const auto& [Key, Value] : ExactPositionRule) {
						if (Value == Room) {
							return true;
						}
					}

					return false;
					});
			}

			// Removes room if it has been used max number of times
			std::erase_if(AvailableRooms, [&](const auto& Room)
				{
					return AlreadySelectedRooms.contains(Room) && AlreadySelectedRooms.at(Room) >= MaxNumberOfRoomsRule.at(Room);
				});

			// If the room has number of entrances critera and doesn't match it, remove it
			std::erase_if(AvailableRooms, [&](const auto& Room)
			{
				int noe = CurrentCell->GetNumberOfEntrances();
				int noer = NumberOfEntrancesRule.contains(Room) ? NumberOfEntrancesRule.at(Room) : 0; 
				if (CurrentCell->GetNumberOfEntrances() == 3 || CurrentCell->GetNumberOfEntrances() == 1)
				{
					if (NumberOfEntrancesRule.contains(Room) && NumberOfEntrancesRule.at(Room) == CurrentCell->GetNumberOfEntrances())
					{
						return false;
					}

					return true;
				}

				return  NumberOfEntrancesRule.contains(Room);
			});

			if (AvailableRooms.size() == 1)
			{
				return AvailableRooms[0];
			}

			// Removes room if it has Direction rules and is in the wrong direction
			std::erase_if(AvailableRooms, [&](const auto& Room)
			{
				if (Y > 0 && Y < (LowResLevelStateData.GetHeight() / 2) - 1)
				{
					return DirectionRule.contains(Room);
				}

				if (DirectionRule.contains(Room)
					&& ((Y < 1 && DirectionRule.at(Room) == DirectionType::North) || (Y > (LowResLevelStateData.GetHeight() / 2) - 1 && DirectionRule.at(Room) == DirectionType::South)))
				{
					return false;
				}

				return true;
			});

			// Select random room
			if (AvailableRooms.empty())
			{
				AvailableRooms.push_back(RoomType::CrewQuarters);
				AvailableRooms.push_back(RoomType::Equipment);
				AvailableRooms.push_back(RoomType::Workshop);
			}

			std::ranges::shuffle(AvailableRooms, Utils::RandomGenerator::GetInstance().GetEngine());
			return AvailableRooms[0];
		}
	}
}