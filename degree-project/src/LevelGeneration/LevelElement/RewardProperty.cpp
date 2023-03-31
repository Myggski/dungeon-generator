#include "RewardProperty.h"

namespace LevelElement
{
	RewardProperty::RewardProperty(LevelElement::RewardType RewardType, RarityType Rarity)
		: RewardType(RewardType),
		Rarity(Rarity) { }

	RewardType RewardProperty::GetRewardType() const
	{
		return RewardType;
	}

	RarityType RewardProperty::GetRarity() const
	{
		return Rarity;
	}
}