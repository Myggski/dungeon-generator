#include "Element.h"

namespace LevelElement
{
	Element::Element(std::unique_ptr<ElementProperty> Property)
		: RoomId(std::numeric_limits<int>::min()),
		Property(std::move(Property)) { }

	Element::Element(const Element& Other)
		: RoomId(Other.RoomId),
		Property(Other.Property->Clone()) { }

	void Element::ConnectToRoom(int RoomIdToConnect)
	{
		RoomId = RoomIdToConnect;
	}

	int Element::GetRoomId() const
	{
		return RoomId;
	}

	ElementProperty& Element::GetProperty() const
	{
		return *Property;
	}

	const type_info& Element::GetPropertyTypeInfo() const
	{
		return typeid(*Property);
	}
}