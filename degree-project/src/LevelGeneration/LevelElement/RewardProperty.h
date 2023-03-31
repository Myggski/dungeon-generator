#pragma once

#include <cstdint>

#include "ElementProperty.h"

namespace LevelElement
{
	enum class RewardType : uint8_t
	{
		Currency = 1 << 1,
		Tech = 1 << 2,
		Resources = 1 << 3,
	};

	enum class RarityType : uint8_t
	{
		Common = 1 << 1,
		Rare = 1 << 2,
		Epic = 1 << 3,
		Legendary = 1 << 4
	};

	struct RewardProperty : public ElementPropertyCRTP<RewardProperty>
	{
	public:
		RewardProperty(RewardType RewardType, RarityType Rarity);
		RewardType GetRewardType() const;
		RarityType GetRarity() const;

	private:
		RewardType RewardType;
		RarityType Rarity;
	};
}
