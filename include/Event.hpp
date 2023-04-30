#pragma once

#include "Defines.hpp"
#include "Debug.hpp"
#include "UnorderedSet.hpp"
#include <initializer_list>

namespace wfe {
	/// @brief A struct that calls multiple listener functions when requested.
	struct Event {
	public:
		/// @brief An event listener function
		typedef void*(*EventListener)(void* args);

		/// @brief Creates an event.
		Event() = default;
		/// @brief Copies the given event.
		/// @param other The event to copy.
		Event(const Event& other) = default;
		/// @brief Moves the given event's contents.
		/// @param other The event whose contents to move.
		Event(Event&& other) noexcept = default;
		/// @brief Creates an event with eveny listener from the list.
		/// @param list The list to copy from.
		Event(std::initializer_list<EventListener> list);

		/// @brief Copies the given event's listeners into this event.
		/// @param other The event to copy from.
		/// @return A reference to this event.
		Event& operator=(const Event& other) = default;
		/// @brief Moves the given event's listeners into this event.
		/// @param other The event to move from.
		/// @return A reference to thsi event.
		Event& operator=(Event&& other) = default;

		/// @brief Gets the number of listeners.
		/// @return The number of listeners.
		size_t GetListenerCount() const;
		/// @brief Gets the event's listeners.
		/// @return An unordered set of the event's listeners.
		unordered_set<EventListener> GetListeners() const;
		/// @brief Adds the given listener to the event.
		/// @param listener The new listener to add to the event.
		/// @return True if a new listener was inserted, otherwise false.
		bool8_t AddListener(EventListener listener);
		/// @brief Removes the given listener from the event.
		/// @param listener The listener to be removed from the event.
		/// @return True if the given listener was removed, otherwise false.
		bool8_t RemoveListener(EventListener listener);
		/// @brief Checks if the event has the given listener.
		/// @param listener THe listener to look for.
		/// @return True if the event already has the given listener, otherwise false.
		bool8_t HasListener(EventListener listener);

		/// @brief Calls every listener of the event.
		/// @param args The args to be passed onto the listeners.
		/// @param returns An optional input vector where all return values will be stored.
		void CallEvent(void* args, void** returns = nullptr);

		// TODO: Implement async call functions

		/// @brief Deletes this event.
		~Event() = default;
	private:
		unordered_set<EventListener> listeners{};
	};
}