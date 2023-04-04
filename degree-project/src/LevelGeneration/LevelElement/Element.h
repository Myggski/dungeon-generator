#pragma once
#include <memory>
#include <string>

#include "ElementProperty.h"

namespace LevelElement
{
	enum class ElementType : uint8_t
	{
		None = 0,
		LockAndKey = 1 << 1,
		MiniBoss = 1 << 2,
		Reward = 1 << 3,
		Trap = 1 << 4,
	};

	struct Element
	{
	public:
		Element(std::unique_ptr<ElementProperty> Property);
		Element(const Element& Other);

		void ConnectToRoom(int RoomId);
		int GetRoomId() const;
		ElementProperty& GetProperty() const;
		std::string GetElementName() const;
		ElementType GetElementType() const;

	private:
		ElementType GetElementTypeFromProperty(ElementProperty& Property) const;

	private:
		int RoomId;
		ElementType ElementType;
		std::unique_ptr<ElementProperty> Property;
	};
}

