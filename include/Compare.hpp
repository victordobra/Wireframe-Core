#pragma once

#include "Defines.hpp"

namespace wfe {
    /// @brief Function struct for performing comparisons.
    /// @tparam T The type to compare.
    template<class T>
    struct LowerComp {
        /// @brief Checks if the first value is lower than the other.
        /// @param val1 The first value in the comparison.
        /// @param val2 The second value in the comparison.
        /// @return True if the first value is lower than the second, otherwise false.
        bool8_t operator()(const T& val1, const T& val2) const {
            return val1 < val2;
        }
    };

    /// @brief Function struct for performing comparisons.
    /// @tparam T The type to compare.
    template<class T>
    struct EqualComp {
        /// @brief Checks if the two given values are equal.
        /// @param val1 The first value in the comparison.
        /// @param val2 The second value in the comparison.
        /// @return True if the two values are equal, otherwise false.
        bool8_t operator()(const T& val1, const T& val2) const {
            return val1 == val2;
        }
    };

    /// @brief Function struct for performing comparisons.
    /// @tparam T The type to compare.
    template<class T>
    struct HigherComp {
        /// @brief Checks if the first value is higher than the other.
        /// @param val1 The first value in the comparison.
        /// @param val2 The second value in the comparison.
        /// @return True if the first value is higher than the second, otherwise false.
        bool8_t operator()(const T& val1, const T& val2) const {
            return val1 > val2;
        }
    };
}