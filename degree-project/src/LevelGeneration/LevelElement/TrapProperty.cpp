#include "TrapProperty.h"

namespace LevelElement
{
	TrapProperty::TrapProperty(TrapType TrapType, TriggerMethod TriggerMethod)
		: Type(TrapType),
		Method(TriggerMethod) { }

	TrapType TrapProperty::GetType() const
	{
		return Type;
	}

	TriggerMethod TrapProperty::GetMethod() const
	{
		return Method;
	}
}