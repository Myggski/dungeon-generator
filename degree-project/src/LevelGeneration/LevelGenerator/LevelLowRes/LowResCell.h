#pragma once

#include <cstdint>
#include <tuple>

namespace LevelGenerator
{

	enum class LowResCellType : uint8_t
	{
		None = 0,
		Room = 1,
		Entrance = 2,
	};

	struct LowResCell
	{
	public:
		LowResCell();
		LowResCell(std::tuple<int, int> Position, LowResCellType Type);

		std::tuple<int, int> GetPosition() const;
		LowResCellType GetType() const;

	private:
		std::tuple<int, int> Position;
		LowResCellType Type;

	};
}
