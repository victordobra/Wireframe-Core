#pragma once

#include "Defines.hpp"
#include "Memory.hpp"

namespace wfe {
    /// @brief Holds a pair of values.
    /// @tparam T1 The type of the first value.
    /// @tparam T2 The type of the second value.
    template<class T1, class T2>
    struct pair {
        /// @brief The first value's type.
        typedef T1 first_type;
        /// @brief THe second value's type.
        typedef T2 second_type;

        /// @brief The first value of the pair.
        first_type first{};
        /// @brief The second value of the pair.
        second_type second{};

        /// @brief Construct a pair with both elements being at their default values.
        constexpr pair() = default;
        /// @brief Copies the given pair.
        /// @param other The pair to copy.
        pair(const pair& other) : first(other.first), second(other.second) { }
        /// @brief Moves the contents of a pair to another pair.
        /// @param other THe pair to move from.
        pair(pair&& other) : first(other.first), second(other.second) { }
        /// @brief Creates a pair from the given values.
        /// @param x The first value in the pair.
        /// @param y The second value in the pair.
        pair(const first_type& x, const second_type& y) : first(x), second(y) { }
        /// @brief Creates a pair from the given values.
        /// @param x The first value in the pair.
        /// @param y The second value in the pair.
        pair(first_type&& x, second_type&& y) : first(x), second(y) { }

        /// @brief Copies the given pair into this pair.
        /// @param other The pair to copy from.
        /// @return A reference to this pair.
        pair& operator=(const pair& other) {
            // Exit the function if both pairs are the same.
            if(&other == this)
                return *this;
            
            // Set both elements
            first = other.first;
            second = other.second;

            return *this;
        }
        /// @brief Moves the given pair's contents into this pair.
        /// @param other THe pair to move from.
        /// @return A reference to this pair.
        pair& operator=(pair&& other) {
            // Move the contents into both elements.
            first = other.first;
            second = other.second;

            return *this;
        }

        /// @brief Swaps the two pair's contents.
        /// @param other THe pair to swap with.
        void swap(pair& other) {
            // Create a memory block to store a pair.
            char_t memory[sizeof(pair)];

            // Copy this pair into the memory block.
            memcpy(memory, this, sizeof(pair));

            // Copy the given pair into this pair.
            memcpy(this, &other, sizeof(pair));

            // Copy the memory block into the given pair
            memcpy(&other, memory, sizeof(pair));
        }

        /// @brief Deletes the pair.
        ~pair() = default;
    };

    /// @brief Compares the two given pairs.
    /// @tparam T1 The type of the pair's first value.
    /// @tparam T2 The type of the pair's second value.
    /// @param pair1 The first pair to compare.
    /// @param pair2 The second pair to compare.
    /// @return True if both pairs are equal, otherwise false.
    template<class T1, class T2>
    bool8_t operator==(const pair<T1, T2>& pair1, const pair<T1, T2>& pair2) {
        return pair1.first == pair2.first && pair1.second == pair2.second;
    }
    /// @brief Compares the two given pairs.
    /// @tparam T1 The type of the pair's first value.
    /// @tparam T2 The type of the pair's second value.
    /// @param pair1 The first pair to compare.
    /// @param pair2 The second pair to compare.
    /// @return True if the pairs are different, otherwise false.
    template<class T1, class T2>
    bool8_t operator!=(const pair<T1, T2>& pair1, const pair<T1, T2>& pair2) {
        return !(pair1.first == pair2.first && pair1.second == pair2.second);
    }
    /// @brief Compares the two given pairs.
    /// @tparam T1 The type of the pair's first value.
    /// @tparam T2 The type of the pair's second value.
    /// @param pair1 The first pair to compare.
    /// @param pair2 The second pair to compare.
    /// @return True if the first pair is smaller than the second, otherwise false.
    template<class T1, class T2>
    bool8_t operator< (const pair<T1, T2>& pair1, const pair<T1, T2>& pair2) {
        if(pair1.first < pair2.first)
            return true;
        return pair1.second < pair2.second;
    }
    /// @brief Compares the two given pairs.
    /// @tparam T1 The type of the pair's first value.
    /// @tparam T2 The type of the pair's second value.
    /// @param pair1 The first pair to compare.
    /// @param pair2 The second pair to compare.
    /// @return True if the first pair is smaller than or equal to the second, otherwise false.
    template<class T1, class T2>
    bool8_t operator<=(const pair<T1, T2>& pair1, const pair<T1, T2>& pair2) {
        if(pair2.first < pair1.first)
            return false;
        return !(pair2.second < pair1.second);
    }
    /// @brief Compares the two given pairs.
    /// @tparam T1 The type of the pair's first value.
    /// @tparam T2 The type of the pair's second value.
    /// @param pair1 The first pair to compare.
    /// @param pair2 The second pair to compare.
    /// @return True if the first pair is bigger than the second, otherwise false.
    template<class T1, class T2>
    bool8_t operator> (const pair<T1, T2>& pair1, const pair<T1, T2>& pair2) {
        if(pair2.first < pair1.first)
            return true;
        return pair2.second < pair1.second;
    }
    /// @brief Compares the two given pairs.
    /// @tparam T1 The type of the pair's first value.
    /// @tparam T2 The type of the pair's second value.
    /// @param pair1 The first pair to compare.
    /// @param pair2 The second pair to compare.
    /// @return True if the first pair is biffer than or equal to the second, otherwise false.
    template<class T1, class T2>
    bool8_t operator>=(const pair<T1, T2>& pair1, const pair<T1, T2>& pair2) {
        if(pair1.first < pair2.first)
            return false;
        return !(pair1.second < pair2.second);
    }
}