#include "LockKeyProperty.h"

namespace LevelElement
{
	LockKeyProperty::LockKeyProperty(LockKeyType Type, int LockRoomDistance)
		: LockRoomDistance(LockRoomDistance),
		Type(Type) { }

	int LockKeyProperty::GetLockRoomDistance() const
	{
		return LockRoomDistance;
	}

	LockKeyType LockKeyProperty::GetType() const
	{
		return Type;
	}
}
