#pragma once

#include <functional>
#include <unordered_map>
#include <SDL_events.h>

namespace Application
{
	using EventCallback = std::function<void(const SDL_Event&)>;
	using EventMap = std::unordered_map<SDL_EventType, std::vector<EventCallback*>>;

	class Events final
	{
	public:
		Events();

		void Pull() const;
		void AddListener(const SDL_EventType& EventType, EventCallback* Callback);
		void remove_listener(const SDL_EventType& EventType, EventCallback* Callback);
		void Clear();

	private:
		void TriggerEvent(const SDL_Event& Event) const;

	private:
		EventMap RegisteredEvents;
	};
}