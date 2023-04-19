#include "Element.h"

#include "LockKeyProperty.h"
#include "MiniBossProperty.h"
#include "RewardProperty.h"
#include "TrapProperty.h"
#include "KillTargetProperty.h"
#include "SecretDocumentProperty.h"

namespace LevelElement
{
	Element::Element(std::unique_ptr<ElementProperty> Property)
		: RoomId(std::numeric_limits<int>::min()),
		ElementType(GetElementTypeFromProperty(*Property)),
		Property(std::move(Property)) { }

	Element::Element(const Element& Other)
		: RoomId(Other.RoomId),
		ElementType(Other.ElementType),
		Property(Other.Property->Clone()) { }

	Element& Element::operator=(const Element& Other)
	{
		RoomId = Other.RoomId;
		ElementType = Other.ElementType;
		Property = Other.Property->Clone();

		return *this;
	}

	Element& Element::operator=(Element&& Other) noexcept
	{
		RoomId = Other.RoomId;
		ElementType = Other.ElementType;
		Property = std::move(Other.Property);

		return *this;
	}

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

	ElementType Element::GetElementTypeFromProperty(ElementProperty& Property) const
	{
		if (dynamic_cast<RewardProperty*>(&Property))
		{
			return ElementType::Reward;
		}

		if (dynamic_cast<MiniBossProperty*>(&Property))
		{
			return ElementType::MiniBoss;
		}

		if (dynamic_cast<LockKeyProperty*>(&Property))
		{
			return ElementType::LockAndKey;
		}

		if (dynamic_cast<TrapProperty*>(&Property))
		{
			return ElementType::Trap;
		}

		if (dynamic_cast<KillTargetProperty*>(&Property))
		{
			return ElementType::KillTarget;
		}

		if (dynamic_cast<SecretDocumentProperty*>(&Property))
		{
			return ElementType::SecretDocument;
		}

		return ElementType::None;
	}

	std::string Element::GetElementName() const
	{
		switch (ElementType)
		{
		case ElementType::LockAndKey:
			return "Lock & Key";
		case ElementType::MiniBoss:
			return "Mini Boss";
		case ElementType::Reward:
			return "Reward";
		case ElementType::Trap:
			return "Trap";
		case ElementType::KillTarget:
			return "Kill Target";
		case ElementType::SecretDocument:
			return "Secret Document";
		case ElementType::None:
		default:
			return "";
		}
	}

	ElementType Element::GetElementType() const
	{
		return ElementType;
	}
}
