#pragma once

#include <cstdint>
#include <tuple>
#include <vector>
#include <memory>

#include <LevelGeneration/LevelGenerator/RuleLevel/RuleLevelCell.h>

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
		LowResCell(std::tuple<int, int> Position, LowResCellType Type, std::vector<std::shared_ptr<LevelElement::Element>> Elements = {});

		void SetEntrance(DirectionType DirectionType);
		bool HasEntrance(DirectionType Direction) const;
		std::tuple<int, int> GetPosition() const;
		const std::vector<std::shared_ptr<LevelElement::Element>>& GetElements() const;
		LowResCellType GetType() const;

	private:
		std::tuple<int, int> Position;
		std::vector<std::shared_ptr<LevelElement::Element>> Elements;
		DirectionType EntranceFlag;
		LowResCellType Type;

	};
}
