#pragma once

#include <cstdint>
#include <memory>

#include "ElementProperty.h"

namespace LevelElement
{
	enum class LockKeyType : uint8_t
	{
		LockKey = 1 << 1,
		LeverBridge = 1 << 2,
		HazardEquipment = 1 << 3
	};

	struct LockKeyProperty : public ElementPropertyCRTP<LockKeyProperty>
	{
	public:
		LockKeyProperty(LockKeyType Type, int LockRoomDistance = 0);

		int GetLockRoomDistance() const;
		LockKeyType GetType() const;

	private:
		// Number of rooms away the lock should be from the key
		// 0 = The lock is in the goal room
		int LockRoomDistance;
		LockKeyType Type;
	};
}