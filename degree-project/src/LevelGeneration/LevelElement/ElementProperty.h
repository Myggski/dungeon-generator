#pragma once

#include <cstdint>
#include <memory>

namespace LevelElement
{
	// To be able to store subclasses of type ElementProperty
	struct ElementProperty
	{
	public:
		virtual ~ElementProperty() = default;

		virtual std::unique_ptr<ElementProperty> Clone() const = 0;
	};

	// Makes sure that all subclasses of ElementProperty has the clone function and acts the same way
	// CRTP = Curiously Recurring Template Pattern
	template<typename T>
	struct ElementPropertyCRTP : public ElementProperty {
	public:
		std::unique_ptr<ElementProperty> Clone() const override {
			return std::make_unique<T>(static_cast<const T&>(*this));
		}
	};
}