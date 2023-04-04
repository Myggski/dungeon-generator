#pragma once

#include <cstdint>

#include "ElementProperty.h"

namespace LevelElement
{
	/**
	 * \brief The traps:
	 * Gravity: A trap that manipulates gravity to either pull the player towards a dangerous
	 * area or push them away from safety. This can be accomplished with a device that creates
	 * a gravitational field or by altering the gravity of the environment itself.
	 *
	 * Laser: A trap that creates a grid of lasers that, when crossed, triggers
	 * an alarm or deals damage to the player.The lasers can be visible or invisible.
	 *
	 * Magnetic Field: A trap that creates a strong magnetic field that can pull the player towards dangerous
	 * objects or repel them away from safety. This can be used to control the player's movement or to manipulate their equipment.
	 */
	enum class TrapType : uint8_t
	{
		Gravity = 1 << 1,
		Lasers = 1 << 2,
		MagneticField = 1 << 3,
	};

	enum class TriggerMethod : uint8_t
	{
		MotionSensor = 1 << 1,
		SoundDetection = 1 << 2,
		PressurePlates = 1 << 3
	};

	struct TrapProperty : public ElementPropertyCRTP<TrapProperty>
	{
	public:
		TrapProperty(TrapType TrapType, TriggerMethod TriggerMethod);
		TrapType GetType() const;
		TriggerMethod GetMethod() const;

	private:
		TrapType Type;
		TriggerMethod Method;
	};
}
