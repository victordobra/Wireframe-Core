#include "Event.hpp"

namespace wfe {
	Event::Event(std::initializer_list<Listener> list) : listeners(list) { }

	size_t Event::GetListenerCount() const {
		return listeners.size();
	}
	Event::Listener* Event::GetListeners() {
		return listeners.data();
	}
	const Event::Listener* Event::GetListeners() const {
		return listeners.data();
	}
	bool8_t Event::AddListener(Listener listener) {
		// Exit the function if the event already has the given listener
		if(HasListener(listener))
			return false;
		
		// Insert the listener into the vector
		listeners.push_back(listener);

		return true;
	}
	bool8_t Event::RemoveListener(Listener listener) {
		// Loop through the vector, checking if the given listener is in the vector
		for(const auto& vecListener : listeners)
			if(vecListener.callback == listener.callback && vecListener.userData == listener.userData) {
				// Erase the current listener and exit the function
				listeners.erase(&vecListener);

				return true;
			}

		return false;
	}
	bool8_t Event::HasListener(Listener listener) {
		// Loop through the vector, checking if the given listener is in the vector
		for(const auto& vecListener : listeners)
			if(vecListener.callback == listener.callback && vecListener.userData == listener.userData)
				return true;

		return false;
	}

	void Event::CallEvent(void* args, void** returns) {
		// Keep and index for setting the appropriate returns
		size_t index = 0;

		// Loop through every listener
		for(auto& listener : listeners) {
			// Call the current listener
			void* result = listener.callback(args, listener.userData);

			// Add the current return to the return vector, if it exists (the index doesn't need to be incremented otherwise)
			if(returns)
				returns[index++] = result;
		}
	}
}