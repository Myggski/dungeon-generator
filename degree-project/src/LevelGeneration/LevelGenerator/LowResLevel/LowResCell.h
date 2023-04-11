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
		Start = 1,
		Room = 2,
		Entrance = 3,
		Goal = 4,
	};

	struct LowResCell
	{
	public:
		LowResCell();
		LowResCell(std::tuple<int, int> Position, LowResCellType Type, std::vector<std::shared_ptr<LevelElement::Element>> Elements = {});

		std::tuple<int, int> GetPosition() const;
		const std::vector<std::shared_ptr<LevelElement::Element>>& GetElements() const;
		LowResCellType GetType() const;

	private:
		std::tuple<int, int> Position;
		std::vector<std::shared_ptr<LevelElement::Element>> Elements;
		LowResCellType Type;

	};
}
