#include "Events.h"

namespace Application
{
	Events::Events() : RegisteredEvents(EventMap()) { }

	void Events::Pull() const
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) {
			TriggerEvent(Event);
		}
	}

	void Events::TriggerEvent(const SDL_Event& Event) const
	{
		if (RegisteredEvents.empty())
		{
			return;
		}

		const auto& Iterator = RegisteredEvents.find(static_cast<SDL_EventType>(Event.type));
		if (Iterator != RegisteredEvents.end())
		{
			const std::vector<EventCallback*>& Callbacks = RegisteredEvents.at(static_cast<SDL_EventType>(Event.type));
			for (const EventCallback* Callback : Callbacks)
			{
				(*Callback)(Event);
			}
		}
	}

	void Events::AddListener(const SDL_EventType& EventType, EventCallback* Callback) {
		if (!RegisteredEvents.contains(EventType))
		{
			RegisteredEvents.insert(std::make_pair(EventType, std::vector<EventCallback*>(1, { Callback })));
		}
		else
		{
			RegisteredEvents.at(EventType).push_back(Callback);
		}
	}

	void Events::remove_listener(const SDL_EventType& EventType, EventCallback* Callback) {

		std::vector<EventCallback*>& Callbacks = RegisteredEvents[EventType];

		auto IsSameEvent = [&](const EventCallback* EventData) {
			return (*EventData).target<EventCallback>() == (*Callback).target<EventCallback>();
		};

		Callbacks.erase(
			std::remove_if(Callbacks.begin(), Callbacks.end(), IsSameEvent), Callbacks.end());

		// Remove event if there's no callbacks
		if (Callbacks.empty())
		{
			RegisteredEvents.erase(RegisteredEvents.find(EventType));
		}
	}

	void Events::Clear()
	{
		if (RegisteredEvents.empty())
		{
			return;
		}

		while (!RegisteredEvents.empty())
		{
			auto& RegisteredEvent = *RegisteredEvents.begin();
			for (const auto& Callback : RegisteredEvent.second)
			{
				remove_listener(RegisteredEvent.first, Callback);
			}
		}

		RegisteredEvents.clear();
	}
}