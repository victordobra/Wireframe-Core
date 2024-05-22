#pragma once

#include "Defines.hpp"
#include "Matrix4x4.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

namespace wfe {
	/// @brief An approximation of pi.
	constexpr float32_t PI = 3.14159265358979323846f;

	/// @brief A multiplier used to convert radians to degrees.
	constexpr float32_t RAD_TO_DEG_MULTIPLIER = 180.f / PI;
	/// @brief A multiplier used to convert degrees to radians.
	constexpr float32_t DEG_TO_RAD_MULTIPLIER = PI / 180.f;

	/// @brief Gets the maximum value between the two input values.
	/// @tparam T The type of the values.
	/// @param val1 The first value.
	/// @param val2 The second value.
	/// @return The maximum value.
	template<class T>
	constexpr T Max(const T& val1, const T& val2) {
		return (val1 < val2) ? val2 : val1;
	}
	/// @brief Gets the minimum value between the two input values.
	/// @tparam T The type of the values.
	/// @param val1 The first value.
	/// @param val2 The second value.
	/// @return The minimum value.
	template<class T>
	constexpr T Min(const T& val1, const T& val2) {
		return (val1 < val2) ? val1 : val2;
	}
	
	/// @brief Calculates the sine of the given number.
	/// @param x The number whose sine to calculate.
	/// @return The given number's sine.
	float32_t Sin(float32_t x);
	/// @brief Calculates the cosine of the given number.
	/// @param x The number whose cosine to calculate.
	/// @return The given number's cosine.
	float32_t Cos(float32_t x);
	/// @brief Calculates the tangent of the given number.
	/// @param x The number whose tangent to calculate.
	/// @return The given number's tangent.
	float32_t Tan(float32_t x);
	/// @brief Calculates the arc sine of the given number.
	/// @param x The number whose arc sine to calculate.
	/// @return The given number's arc sine.
	float32_t Asin(float32_t x);
	/// @brief Calculates the arc cosine of the given number.
	/// @param x The number whose arc cosine to calculate.
	/// @return The given number's arc cosine.
	float32_t Acos(float32_t x);
	/// @brief Calculates the arc tangent of the given number.
	/// @param x The number whose arc tangent to calculate.
	/// @return The given number's arc tangent.
	float32_t Atan(float32_t x);

	/// @brief Calculates the square root of the given number.
	/// @param x The number whose square root to calculate.
	/// @return The given number's square root.
	float32_t Sqrt(float32_t x);
	/// @brief Calculates the inverse square root of the given number using the fast inverse square root alogrithm.
	/// @param x The number whose inverse square root to calculate.
	/// @return The given number's inverse square root.
	float32_t InvSqrt(float32_t x);

	/// @brief Calculates the absolute value of the given number.
	/// @param x The number whose absolute value to calculate.
	/// @return The given number's absolute value.
	float32_t Absolute(float32_t x);

	/// @brief Checks if the given number is a power of 2.
	/// @param x The number to check.
	/// @return True if the given number is a power of 2, otherwise false.
	bool8_t IsPowerOf2(uint64_t x);
	/// @brief Gets the popcount of the given number, meaning the number of bits set to 1 in the number.
	/// @param x The number to count in.
	/// @return The popcount of the given number.
	uint64_t Popcount(uint64_t x);
	/// @brief Finds the leftmost (most significant) bit of the given number.
	/// @param x The number to find in.
	/// @return A mask of the leftmost bit of the given number.
	uint64_t LeftmostBit(uint64_t x);
	/// @brief Finds the rightmost (least significant) bit of the given number.
	/// @param x The number to find in.
	/// @return A mask of the rightmost bit of the given number.
	uint64_t RightmostBit(uint64_t x);
	/// @brief Gets the index of a power of 2's bit.
	/// @param x The number to find in.
	/// @return The index of the bit. Undefined for non-powers of 2.
	uint64_t Pow2BitIndex(uint64_t x);

	/// @brief Sets the random number generator's seed.
	/// @param seed The random number generator's new seed.
	void RandomSeed(uint32_t seed);

	/// @brief Generates a random integer.
	/// @return A random integer.
	int32_t Random();
	/// @brief Generates a random integer in the given range.
	/// @param min The minimum of the range.
	/// @param max The maximum of the range.
	/// @return A random integer in the given range.
	int32_t RandomInRange(int32_t min, int32_t max);
	/// @brief Generates a random floating point number.
	/// @return A random floating point number.
	float32_t RandomFloat();
	/// @brief Generates a random floating pointer number in the given range.
	/// @param min The minimum of the range.
	/// @param max The maximum of the range.
	/// @return A random floating point number in the given range.
	float32_t RandomFloatInRange(float32_t min, float32_t max);

	/// @brief A function struct hsed for hashing a specific type.
	/// @tparam T The type of the values to hash.
	template<class T>
	struct Hash {
		/// @brief Hashes the given value.
		/// @param val The value to hash.
		/// @return A 64-bit hash of the given value.
		uint64_t operator()(const T& val) const;
	};

	/// @brief Combines the two given hashes.
	/// @param hash1 The first hash to combine.
	/// @param hash2 The second hash to combine.
	/// @return The resulting hash.
	uint64_t HashCombine(uint64_t hash1, uint64_t hash2);
}