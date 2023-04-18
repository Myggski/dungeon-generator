#pragma once

#include <cstdint>

namespace LevelGenerator
{
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
		HoldingBay = 1 << 12,
		Hub = 1 << 13,
	};

	inline std::string RoomTypeToString(RoomType Type)
	{
		switch (Type)
		{
		case RoomType::Cargo:
			return "Cargo";
		case RoomType::CrewQuarters:
			return "Crew Quarters";
		case RoomType::MedicalBay:
			return "Medical Bay";
		case RoomType::Bridge:
			return "Bridge";
		case RoomType::ScienceLab:
			return "Science Lab";
		case RoomType::Communication:
			return "Communication";
		case RoomType::Equipment:
			return "Equipment";
		case RoomType::Laundry:
			return "Laundry Room";
		case RoomType::Exercise:
			return "Gym";
		case RoomType::EscapePods:
			return "Escape Pods";
		case RoomType::Workshop:
			return "Workshop";
		case RoomType::HoldingBay:
			return "Holding Bay";
		case RoomType::Hub:
			return "Hub";
		default:
			return "";
		}
	}
}