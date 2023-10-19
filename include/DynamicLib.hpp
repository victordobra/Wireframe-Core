#pragma once

#include "Defines.hpp"

namespace wfe {
    /// @brief A struct implementing a dynamic library.
    struct DynamicLib {
    public:
        /// @brief The type used to define a pointer to a function.
        typedef void(*VoidFunction)();

        /// @brief Creates the dynamic lib struct without any dynamic lib loaded.
        DynamicLib() = default;
        DynamicLib(const DynamicLib&) = delete;
        DynamicLib(DynamicLib&&) noexcept = delete;

        /// @brief Loads the dynamic lib with the given name into this struct.
        /// @param name The name of the dynamic lib to load.
        /// @return True if the library was loaded successfully, otherwise false.
        bool8_t LoadLib(const char_t* name);
        /// @brief Loads the function with the given name from the dynamic lib.
        /// @param name The name of the function to load.
        /// @return A pointer to the loaded function, or a nullptr if the load failed.
        VoidFunction LoadFunc(const char_t* name);
        /// @brief Frees the loaded dynamic lib.
        void FreeLib();

        /// @brief Frees the loaded dynamic lib.
        ~DynamicLib();
    private:
        void* internalData = nullptr;
    };
}