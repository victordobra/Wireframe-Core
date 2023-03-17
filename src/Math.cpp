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
		uint32_t i;
		float32_t halfX = x * .5f, invSqrtX = x;

		// Copy the float's value into an unsigned int
		i = *(uint32_t*)&invSqrtX;

		// Perform dark magic
		i = 0x5f3759df - (i >> 1);

		// Copy the unsigned int's value into a float
		invSqrtX = *(float32_t*)&i;

		// Apply two Newton iterations
		invSqrtX = invSqrtX * (1.5f - (halfX * invSqrtX * invSqrtX));
		invSqrtX = invSqrtX * (1.5f - (halfX * invSqrtX * invSqrtX));

		return invSqrtX;
	}

	float32_t Absolute(float32_t x) {
		// Apply an IEEE 754 bit hack to the number
		(*(uint32_t*)&x) &= ~(1 << 31);

		// Return the number
		return x;
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
	const uint64_t LARGE_PRIME = 0x1000193;

	template<>
	Hash::Hash(const uint8_t& val) {
		this->val = (uint64_t)val;
	}
	template<>
	Hash::Hash(const uint16_t& val) {
		this->val = (uint64_t)val;
	}
	template<>
	Hash::Hash(const uint32_t& val) {
		this->val = (uint64_t)val;
	}
	template<>
	Hash::Hash(const uint64_t& val) {
		this->val = val;
	}
	template<>
	Hash::Hash(const int8_t& val) {
		this->val = (uint64_t)val;
	}
	template<>
	Hash::Hash(const int16_t& val) {
		this->val = (uint64_t)val;
	}
	template<>
	Hash::Hash(const int32_t& val) {
		this->val = (uint64_t)val;
	}
	template<>
	Hash::Hash(const int64_t& val) {
		this->val = (uint64_t)val;
	}
	template<>
	Hash::Hash(const float32_t& val) {
		this->val = (uint64_t)*(uint32_t*)&val;
	}
	template<>
	Hash::Hash(const float64_t& val) {
		this->val = *(uint64_t*)&val;
	}
	template<>
	Hash::Hash(const string& val) {
		// Apply a common string hashing algorithm
		this->val = 0x811c9dc5;

		size_t size = val.length();
		const char_t* strPtr = val.data();

		while(size >= sizeof(size_t)) {
			// Save a size_t segment
			size_t segment = *(const size_t*)strPtr;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Loop through every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Add the current character to the hash
				this->val ^= *segmentPtr;
				this->val *= LARGE_PRIME;

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
				this->val ^= *segmentPtr;
				this->val *= LARGE_PRIME;

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
				this->val ^= *segmentPtr;
				this->val *= LARGE_PRIME;

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
			this->val ^= *strPtr;
			this->val *= LARGE_PRIME;
		}
	}
	Hash::Hash(const char_t* val) {
		// Apply a common string hashing algorithm
		this->val = 0x811c9dc5;

		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)val;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Loop through every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the string has ended.
				if(!*segmentPtr)
					return;
				
				// Add the current character to the hash
				this->val ^= *segmentPtr;
				this->val *= LARGE_PRIME;

				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			val += sizeof(size_t);
		}
	}

	Hash::operator uint64_t() const {
		return val;
	}

	Hash& Hash::operator|=(const Hash& other) {
		val ^= other.val + 0x9e3779b9 + (val << 6) + (val >> 2);

		return *this;
	}

	Hash Hash::operator|(const Hash& other) {
		return { val ^ (other.val + 0x9e3779b9 + (val << 6) + (val >> 2)) };
	}
}