#pragma once

#include "Defines.hpp"
#include "Memory.hpp"

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

    /// @brief Checks if the first string is lower than the other.
    /// @param str1 The first string in the comparison.
    /// @param str2 The second string in the comparison.
    /// @return True if the first string is lower than the second, otherwise false.
    template<>
    WFE_INLINE bool8_t LowerComp<const char_t*>::operator()(const char_t* const& str1, const char_t* const& str2) const {
        return strcmp(str1, str2) < 0;
    }
    /// @brief Checks if the two given strings are equal.
    /// @param str1 The first string in the comparison.
    /// @param str2 The second string in the comparison.
    /// @return True if the two strings are equal, otherwise false.
    template<>
    WFE_INLINE bool8_t EqualComp<const char_t*>::operator()(const char_t* const& str1, const char_t* const& str2) const {
        return !strcmp(str1, str2);
    }
    /// @brief Checks if the first string is higher than the other.
    /// @param str1 The first string in the comparison.
    /// @param str2 The second string in the comparison.
    /// @return True if the first string is higher than the second, otherwise false.
    template<>
    WFE_INLINE bool8_t HigherComp<const char_t*>::operator()(const char_t* const& str1, const char_t* const& str2) const {
        return strcmp(str1, str2) > 0;
    }
}