#include "events.h"

namespace application
{
	events::events() : registered_events(event_map()) { }

	void events::pull() const
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			trigger_event(ev);
		}
	}

	void events::trigger_event(const SDL_Event& event) const
	{
		if (registered_events.empty())
		{
			return;
		}

		const auto& it = registered_events.find(static_cast<SDL_EventType>(event.type));
		if (it != registered_events.end())
		{
			const std::vector<event_callback*>& callbacks = registered_events.at(static_cast<SDL_EventType>(event.type));
			for (const event_callback* callback : callbacks)
			{
				(*callback)(event);
			}
		}
	}

	void events::add_listener(const SDL_EventType& eventType, event_callback* callback) {
		if (!registered_events.count(eventType))
		{
			registered_events.insert(std::make_pair(eventType, std::vector<event_callback*>(1, { callback })));
		}
		else
		{
			registered_events.at(eventType).push_back(callback);
		}
	}

	void events::remove_listener(const SDL_EventType& eventType, event_callback* callback) {

		std::vector<event_callback*>& callbacks = registered_events[eventType];

		auto is_same_callback = [&](const event_callback* event_data) {
			return (*event_data).target<event_callback>() == (*callback).target<event_callback>();
		};

		callbacks.erase(
			std::remove_if(callbacks.begin(), callbacks.end(), is_same_callback), callbacks.end());

		// Remove event if there's no callbacks
		if (callbacks.empty())
		{
			registered_events.erase(registered_events.find(eventType));
		}
	}

	void events::clear()
	{
		if (registered_events.empty())
		{
			return;
		}

		while (!registered_events.empty())
		{
			auto& registered_event = *registered_events.begin();
			for (const auto& callback : registered_event.second)
			{
				remove_listener(registered_event.first, callback);
			}
		}

		registered_events.clear();
	}
}