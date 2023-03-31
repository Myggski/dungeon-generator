#pragma once
#include <memory>

#include "ElementProperty.h"

namespace LevelElement
{
	struct Element
	{
	public:
		Element(std::unique_ptr<ElementProperty> Property);
		Element(const Element& Other);

		void ConnectToRoom(int RoomId);
		int GetRoomId() const;
		const type_info& GetPropertyTypeInfo() const;
		ElementProperty& GetProperty() const;

	private:
		int RoomId;
		std::unique_ptr<ElementProperty> Property;
	};
}

