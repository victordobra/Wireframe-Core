#include "Event.hpp"

namespace wfe {
    Event::Event(std::initializer_list<EventListener> list) : listeners(list) { }

    size_t Event::GetListenerCount() const {
        // Return the listener count
        return listeners.size();
    }
    unordered_set<Event::EventListener> Event::GetListeners() const {
        // Return the listener unordered set
        return listeners;
    }
    bool8_t Event::AddListener(EventListener listener) {
        // Insert the listener into the unordered set
        auto result = listeners.insert(listener);

        // Return the insert function's returned bool
        return result.second;
    }
    bool8_t Event::RemoveListener(EventListener listener) {
        // Return the erase function's result, since it will either return 0 and 1
        return listeners.erase(listener);
    }
    bool8_t Event::HasListener(EventListener listener) {
        // The unordered set's find function will return end() if the given walue isn't in the set; compare the return value with end()
        return listeners.find(listener) != listeners.end();
    }

    void Event::CallEvent(void* args, void** returns) {
        // Keep and index for setting the appropriate returns
        size_t index = 0;

        // Loop through every listener
        for(auto& listener : listeners) {
            // Call the current listener
            void* result = listener(args);

            // Add the current return to the return vector, if it exists (the index doesn't need to be incremented otherwise)
            if(returns)
                returns[index++] = result;
        }
    }
}