#include "Math.hpp"
#include "String.hpp"
#include <math.h>

namespace wfe {
	float32_t Sin(float32_t x) {
		return sin(x);
	}
	float32_t Cos(float32_t x) {
		return cos(x);
	}
	float32_t Tan(float32_t x) {
		return tan(x);
	}
	float32_t Asin(float32_t x) {
		return asin(x);
	}
	float32_t Acos(float32_t x) {
		return acos(x);
	}
	float32_t Atan(float32_t x) {
		return atan(x);
	}

	float32_t Sqrt(float32_t x) {
		return sqrt(x);
	}
	float32_t InvSqrt(float32_t x) {
		// Declare an integer and the given number in an union to apply bithacks
		union {
			uint32_t intInvSqrtX;
			float32_t invSqrtX;
		} sqrtUnion;
		sqrtUnion.invSqrtX = x;

		// Perform dark magic
		sqrtUnion.intInvSqrtX = 0x5f3759df - (sqrtUnion.intInvSqrtX >> 1);

		// Apply two Newton iterations
		float32_t halfX = x * .5f;
		sqrtUnion.invSqrtX = sqrtUnion.invSqrtX * (1.5f - (halfX * sqrtUnion.invSqrtX * sqrtUnion.invSqrtX));
		sqrtUnion.invSqrtX = sqrtUnion.invSqrtX * (1.5f - (halfX * sqrtUnion.invSqrtX * sqrtUnion.invSqrtX));

		return sqrtUnion.invSqrtX;
	}

	float32_t Absolute(float32_t x) {
		// Declare an integer and the given number in an union to apply bithacks
		union {
			uint32_t intAbsX;
			float32_t absX;
		} absUnion;
		absUnion.absX = x;

		// Apply an IEEE 754 bit hack to the number
		absUnion.intAbsX &= ~(1 << 31);

		// Return the number
		return absUnion.absX;
	}

	bool8_t IsPowerOf2(uint64_t x) {
		return x && !(x & (x - 1));
	}

	void RandomSeed(uint32_t seed) {
		srand(seed);
	}
	int32_t Random() {
		return rand();
	}
	int32_t RandomInRange(int32_t min, int32_t max) {
		return (rand() % (max - min + 1)) + min;
	}
	float32_t RandomFloat() {
		return (float32_t)Random() / (float32_t)RAND_MAX;
	}
	float32_t RandomFloatInRange(float32_t min, float32_t max) {
		return ((float32_t)Random() / ((float32_t)RAND_MAX / (max - min))) + min;
	}

	/// @brief A large prime used for hashing strings.
	static const uint64_t LARGE_PRIME = 0x9ddfea08eb382d69;

	// Internal integer hashing function
	static uint64_t HashInt(uint64_t val) {
		// Use an example hash algorithm from https://github.com/skeeto/hash-prospector/tree/master
		val += 0x9e3779b97f4a7c15;
		val ^= val >> 30;
		val *= 0xbf58476d1ce4e5b9;
		val ^= val >> 27;
		val *= 0x94d049bb133111eb;
		val ^= val >> 31;

		return val;
	}

	template<>
	uint64_t Hash<uint8_t>::operator()(const uint8_t& val) const {
		return HashInt(val);
	}
	template<>
	uint64_t Hash<uint16_t>::operator()(const uint16_t& val) const {
		return HashInt(val);
	}
	template<>
	uint64_t Hash<uint32_t>::operator()(const uint32_t& val) const {
		return HashInt(val);
	}
	template<>
	uint64_t Hash<uint64_t>::operator()(const uint64_t& val) const {
		return HashInt(val);
	}
	template<>
	uint64_t Hash<int8_t>::operator()(const int8_t& val) const {
		return HashInt((uint64_t)val);
	}
	template<>
	uint64_t Hash<int16_t>::operator()(const int16_t& val) const {
		return HashInt((uint64_t)val);
	}
	template<>
	uint64_t Hash<int32_t>::operator()(const int32_t& val) const {
		return HashInt((uint64_t)val);
	}
	template<>
	uint64_t Hash<int64_t>::operator()(const int64_t& val) const {
		return HashInt((uint64_t)val);
	}
	template<>
	uint64_t Hash<float32_t>::operator()(const float32_t& val) const {
		return HashInt(*(uint32_t*)&val);
	}
	template<>
	uint64_t Hash<float64_t>::operator()(const float64_t& val) const {
		return HashInt(*(uint64_t*)&val);
	}
	template<>
	uint64_t Hash<string>::operator()(const string& val) const {
		// Apply a common string hashing algorithm
		uint64_t hash = 0x811c9dc5;

		size_t size = val.length();
		const char_t* strPtr = val.data();

		while(size >= sizeof(size_t)) {
			// Save a size_t segment
			size_t segment = *(const size_t*)strPtr;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Loop through every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Add the current character to the hash
				hash ^= *segmentPtr;
				hash *= LARGE_PRIME;

				// Increment the segment pointer
				++segmentPtr;
			}

			// Decrement the string's size by the size of size_t
			size -= sizeof(size_t);

			// Increment the string pointer by the size of size_t
			strPtr += sizeof(size_t);
		}
#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to loop through
		if(size >= sizeof(uint32_t)) {
			// Save a uint32_t segment
			uint32_t segment = *(const uint32_t*)strPtr;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Loop through every character in the segment
			for(size_t i = 0; i != sizeof(uint32_t); ++i) {
				// Add the current character to the hash
				hash ^= *segmentPtr;
				hash *= LARGE_PRIME;

				// Increment the segment pointer
				++segmentPtr;
			}

			// Decrement the string's size by the size of uint32_t
			size -= sizeof(uint32_t);

			// Increment the string pointer by the size of uint32_t
			strPtr += sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to loop through
		if(size >= sizeof(uint16_t)) {
			// Save a uint16_t segment
			uint16_t segment = *(const uint16_t*)strPtr;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Loop through every character in the segment
			for(size_t i = 0; i != sizeof(uint16_t); ++i) {
				// Add the current character to the hash
				hash ^= *segmentPtr;
				hash *= LARGE_PRIME;

				// Increment the segment pointer
				++segmentPtr;
			}

			// Decrement the string's size by the size of uint16_t
			size -= sizeof(uint16_t);

			// Increment the string pointer by the size of uint16_t
			strPtr += sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to loop through
		if(size) {
			// Add the current character to the hash
			hash ^= *strPtr;
			hash *= LARGE_PRIME;
		}

		return hash;
	}
	template<>
	uint64_t Hash<const char_t*>::operator()(const char_t* const& val) const {
		// Apply a common string hashing algorithm
		const char_t* str = val;
		uint64_t hash = 0x811c9dc5;

		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Loop through every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the string has ended.
				if(!*segmentPtr)
					return hash;
				
				// Add the current character to the hash
				hash ^= *segmentPtr;
				hash *= LARGE_PRIME;

				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		return hash;
	}

	uint64_t HashCombine(uint64_t hash1, uint64_t hash2) {
		// Use the hash combination algorithm from https://github.com/google/cityhash
		uint64_t a = (hash1 ^ hash2) * LARGE_PRIME;
		a ^= a >> 47;
		uint64_t b = (hash1 ^ a) * LARGE_PRIME;
		b ^= b >> 47;
		return b * LARGE_PRIME;
	}
}