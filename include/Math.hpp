#pragma once

#include "Defines.hpp"
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

	/// @brief Holds a hash.
	struct Hash {
		/// @brief Copies the given hash.
		/// @param other The hash to copy.
		Hash(const Hash& other) = default;
		/// @brief Copies the given hash.
		/// @param other The hash to copy.
		Hash(Hash&& other) noexcept = default;
		/// @brief Hashes the given value.
		/// @tparam T The given value's type.
		/// @param val The value to hash.
		template<class T>
		Hash(const T& val);
		/// @brief Hashes the given pointer.
		/// @tparam T The given pointer's type.
		/// @param val The pointer to hash.
		template<class T>
		Hash(const T* val) {
			val = (uint64_t*)val;
		}
		/// @brief Hashes the given string.
		/// @param val The string to hash.
		Hash(const char_t* val);

		/// @brief Copies the given hash into this hash.
		/// @param other The hash to copy.
		/// @return A reference to this hash.
		Hash& operator=(const Hash& other) = default;
		/// @brief Copies the given hash into this hash.
		/// @param other The hash to copy.
		/// @return A reference to this hash.
		Hash& operator=(Hash&& other) = default;

		/// @brief Gets the hash's value.
		operator uint64_t() const;

		/// @brief Combines the given hash into this hash.
		/// @param other The hash to combine.
		/// @return A reference to this hash.
		Hash& operator|=(const Hash& other);

		/// @brief Combines the given hash with this hash.
		/// @param other The hash to combine with.
		/// @return The resulting hash.
		Hash operator|(const Hash& other);

		/// @brief Destruct the hash.
		~Hash() = default;

		/// @brief The hash's value.
		uint64_t val = 0;		
	};
}