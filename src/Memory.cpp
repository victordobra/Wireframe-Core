#include "Memory.hpp"
#include "Platform.hpp"

namespace wfe {
	void* memcpy(void* dest, const void* src, size_t size) {
		// Exit if the target and source memory block are the same; nothing needs to be copied
		if(dest == src)
			return dest;

		// Convert every given pointer to char pointers
		char_t* destChar = (char_t*)dest;
		const char_t* srcChar = (const char_t*)src;

		// Copy everything in segments of size_t
		while(size >= sizeof(size_t)) {
			// Copy one size_t segment
			*(size_t*)destChar = *(size_t*)srcChar;

			// Increment the pointers by the size of size_t
			destChar += sizeof(size_t);
			srcChar += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to copy
		if(size >= sizeof(uint32_t)) {
			// Copy one uint32_t segment
			*(uint32_t*)destChar = *(uint32_t*)srcChar;

			// Increment the pointers by the size of uint32_t
			destChar += sizeof(uint32_t);
			srcChar += sizeof(uint32_t);

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to copy
		if(size >= sizeof(uint16_t)) {
			// Copy one uint16_t segment
			*(uint16_t*)destChar = *(uint16_t*)srcChar;

			// Increment the pointers by the size of uint16_t
			destChar += sizeof(uint16_t);
			srcChar += sizeof(uint16_t);

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to copy
		if(size) {
			// Copy one uint8_t segment
			*(uint8_t*)destChar = *(uint8_t*)srcChar;
		}

		return dest;
	}
	void* memmove(void* dest, const void* src, size_t size) {
		// Exit if the target and source memory block are the same; nothing needs to be copied
		if(dest == src)
			return dest;
		else if(dest < src) {
			// Copy the memory blocks from left to right; memcpy works fine in this case
			return memcpy(dest, src, size);
		} else {
			// Copy the memory blocks from right to left
			char_t* destChar = (char_t*)dest + size;
			const char_t* srcChar = (const char_t*)src + size;

			// Copy everything in segments of size_t
			while(size >= sizeof(size_t)) {
				// Decrement the pointers by the size of size_t
				destChar -= sizeof(size_t);
				srcChar -= sizeof(size_t);

				// Copy one size_t segment
				*(size_t*)destChar = *(size_t*)srcChar;

				// Decrement the size by the size of size_t
				size -= sizeof(size_t);
			}

#if defined(WFE_ARCHITECTURE_X64)
			// Check if there is a 4 byte region left to copy
			if(size >= sizeof(uint32_t)) {
				// Decrement the pointers by the size of uint32_t
				destChar -= sizeof(uint32_t);
				srcChar -= sizeof(uint32_t);

				// Copy one uint32_t segment
				*(uint32_t*)destChar = *(uint32_t*)srcChar;

				// Decrement the size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
#endif
			// Check if there is a 2 byte region left to copy
			if(size >= sizeof(uint16_t)) {
				// Decrement the pointers by the size of uint16_t
				destChar -= sizeof(uint16_t);
				srcChar -= sizeof(uint16_t);

				// Copy one uint16_t segment
				*(uint16_t*)destChar = *(uint16_t*)srcChar;

				// Decrement the size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
			// Check if there is a 1 byte region left to copy
			if(size) {
				// Decrement the pointers by the size of uint8_t
				destChar -= sizeof(uint8_t);
				srcChar -= sizeof(uint8_t);

				// Copy one uint8_t segment
				*(uint8_t*)destChar = *(uint8_t*)srcChar;
			}

			return dest;
		}
	}
	void* memccpy(void* dest, const void* src, int32_t c, size_t size) {
		// Exit if the target and source memory block are the same; nothing needs to be copied
		if(dest == src)
			return dest;

		// Convert every given pointer to char pointers
		char_t* destChar = (char_t*)dest;
		const char_t* srcChar = (const char_t*)src;

		// Copy everything in segments of size_t
		while(size >= sizeof(size_t)) {
			// Save a size_t segment 
			size_t segment = *(size_t*)srcChar;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Try to match the character with every byte in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				if(*segmentPtr == (char_t)c) {
					// Check if it's the last bit in the segment
					if(i == sizeof(size_t) - 1) {
						// Copy the segment into the destination folder and exit the function
						*(size_t*)destChar = segment;
						return destChar + i;
					}

					// Save the size lower by 1 to avoid edge cases
					size = i;
					break;
				}

				// Increment the segment pointer
				++segmentPtr;
			}
			// Check if the character was found
			if(size < sizeof(size_t)) {
				// Increment the size back and exit the loop
				++size;
				break;
			}

			// Copy the size_t segment
			*(size_t*)destChar = segment;

			// Increment the pointers by the size of size_t
			destChar += sizeof(size_t);
			srcChar += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to copy
		if(size >= sizeof(uint32_t)) {
			// Save an uint32_t segment 
			uint32_t segment = *(uint32_t*)srcChar;

			// Try to match the character with every byte in the segment
			char_t* segmentPtr = (char_t*)&segment;
			for(size_t i = 0; i != sizeof(uint32_t); ++i) {
				if(*segmentPtr == (char_t)c) {
					// Check if it's the last bit in the segment
					if(i == sizeof(uint32_t) - 1) {
						// Copy the segment into the destination folder and exit the function
						*(uint32_t*)destChar = segment;
						return destChar + i;
					}

					// Save the size lower by 1 to avoid edge cases
					size = i;
					break;
				}
				++segmentPtr;
			}

			// Check if the character was found
			if(size < sizeof(uint32_t)) {
				// Increment the size back and exit the loop
				++size;
			} else {
				// Copy one uint32_t segment
				*(uint32_t*)destChar = *(uint32_t*)srcChar;

				// Increment the pointers by the size of uint32_t
				destChar += sizeof(uint32_t);
				srcChar += sizeof(uint32_t);

				// Decrement the size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
		}
#endif
		// Check if there is a 2 byte region left to copy
		if(size >= sizeof(uint16_t)) {
			// Save an uint16_t segment 
			uint16_t segment = *(uint16_t*)srcChar;

			// Try to match the character with every byte in the segment
			char_t* segmentPtr = (char_t*)&segment;
			for(size_t i = 0; i != sizeof(uint16_t); ++i) {
				if(*segmentPtr == (char_t)c) {
					// Check if it's the last bit in the segment
					if(i == sizeof(uint16_t) - 1) {
						// Copy the segment into the destination folder and exit the function
						*(uint16_t*)destChar = segment;
						return destChar + i;
					}

					// Save the size lower by 1 to avoid edge cases
					size = i;
					break;
				}
				++segmentPtr;
			}

			// Check if the character was found
			if(size < sizeof(uint16_t)) {
				// Increment the size back and exit the loop
				++size;
			} else {
				// Copy one uint16_t segment
				*(uint16_t*)destChar = *(uint16_t*)srcChar;

				// Increment the pointers by the size of uint16_t
				destChar += sizeof(uint16_t);
				srcChar += sizeof(uint16_t);

				// Decrement the size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
		}
		// Check if there is a 1 byte region left to copy
		if(size) {
			// Save an uint8_t segment
			uint8_t segment = (uint8_t)*srcChar;

			// Copy the segment
			*(uint8_t*)destChar = segment;

			if(segment == (uint8_t)c)
				return destChar;
		}

		return nullptr;
	}
	void* memset(void* ptr, int32_t c, size_t size) {
		// Convert the given pointer to a char pointer
		char_t* ptrChar = (char_t*)ptr;

		// Generate a size_t segment with all bytes set to c's value
		size_t segment = 0;
		for(size_t i = 0; i != sizeof(size_t); ++i)
			segment |= ((size_t)(uint8_t)c) << (8 * i);
		
		// Set everything in segments of size_t
		while(size >= sizeof(size_t)) {
			// Set one size_t segment
			*(size_t*)ptrChar = segment;

			// Increment the pointer by the size of size_t
			ptrChar += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to set
		if(size >= sizeof(uint32_t)) {
			// Set one uint32_t segment
			*(uint32_t*)ptrChar = (uint32_t)segment;

			// Increment the pointer by the size of uint32_t
			ptrChar += sizeof(uint32_t);

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to set
		if(size >= sizeof(uint16_t)) {
			// Set one uint16_t segment
			*(uint16_t*)ptrChar = (uint16_t)segment;

			// Increment the pointer by the size of uint16_t
			ptrChar += sizeof(uint16_t);

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to set
		if(size) {
			// Copy one uint8_t segment
			*(uint8_t*)ptrChar = (uint8_t)segment;
		}

		return ptr;
	}

	int32_t memcmp(const void* ptr1, const void* ptr2, size_t size) {
		// Exit if the memory blocks are the same; nothing needs to be compared
		if(ptr1 == ptr2)
			return 0;
		
		// Convert every given pointer to char pointers
		const char_t* ptr1Char = (const char_t*)ptr1;
		const char_t* ptr2Char = (const char_t*)ptr2;
		
		// Compare every memory block in segments of size_t
		while(size >= sizeof(size_t)) {
			// Save a size_t segment from both memory blocks
			size_t segment1 = *(const size_t*)ptr1Char;
			size_t segment2 = *(const size_t*)ptr2Char;

			const char_t* segment1Ptr = (const char_t*)&segment1;
			const char_t* segment2Ptr = (const char_t*)&segment2;

			// Compare all characters
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Compare both characters
				if(*segment1Ptr > *segment2Ptr)
					return 1;
				else if(*segment1Ptr < *segment2Ptr)
					return -1;

				// Increment both pointers
				++segment1Ptr;
				++segment2Ptr;
			}

			// Increment both pointers by the size of size_t
			ptr1Char += sizeof(size_t);
			ptr2Char += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to compare
		if(size >= sizeof(uint32_t)) {
			// Save a uint32_t segment from both memory blocks
			uint32_t segment1 = *(const uint32_t*)ptr1Char;
			uint32_t segment2 = *(const uint32_t*)ptr2Char;

			const char_t* segment1Ptr = (const char_t*)&segment1;
			const char_t* segment2Ptr = (const char_t*)&segment2;

			// Compare all characters
			for(size_t i = 0; i != sizeof(uint32_t); ++i) {
				// Compare both characters
				if(*segment1Ptr < *segment2Ptr)
					return -1;
				else if(*segment1Ptr > *segment2Ptr)
					return 1;

				// Increment both pointers
				++segment1Ptr;
				++segment2Ptr;
			}

			// Increment both pointers by the size of uint32_t
			ptr1Char += sizeof(uint32_t);
			ptr2Char += sizeof(uint32_t);

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to compare
		if(size >= sizeof(uint16_t)) {
			// Save a uint16_t segment from both memory blocks
			uint16_t segment1 = *(const uint16_t*)ptr1Char;
			uint16_t segment2 = *(const uint16_t*)ptr2Char;

			const char_t* segment1Ptr = (const char_t*)&segment1;
			const char_t* segment2Ptr = (const char_t*)&segment2;

			// Compare all characters
			for(size_t i = 0; i != sizeof(uint16_t); ++i) {
				// Compare both characters
				if(*segment1Ptr < *segment2Ptr)
					return -1;
				else if(*segment1Ptr > *segment2Ptr)
					return 1;

				// Increment both pointers
				++segment1Ptr;
				++segment2Ptr;
			}

			// Increment both pointers by the size of uint16_t
			ptr1Char += sizeof(uint16_t);
			ptr2Char += sizeof(uint16_t);

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to compare
		if(size) {
			// Save the two current characters
			char_t char1 = *ptr1Char, char2 = *ptr2Char;

			// Compare the current characters
			if(char1 < char2)
				return -1;
			else if(char1 > char2)
				return 1;
		}

		return 0;
	}
	void* memchr(void* ptr, int32_t c, size_t size) {
		// Convert the given pointer to a char pointer
		char_t* ptrChar = (char_t*)ptr;

		// Search for the character in size_t segments
		while(size >= sizeof(size_t)) {
			// Save a size_t segment
			size_t segment = *(size_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Increment the pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of size_t
			ptrChar += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to search in
		if(size >= sizeof(uint32_t)) {
			// Save an uint32_t segment
			uint32_t segment = *(uint32_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;
			for(size_t i = 0; i != sizeof(uint32_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Increment the pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of uint32_t
			ptrChar += sizeof(uint32_t);

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to search in
		if(size >= sizeof(uint16_t)) {
			// Save an uint16_t segment
			uint16_t segment = *(uint16_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;
			for(size_t i = 0; i != sizeof(uint16_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Increment the pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of uint16_t
			ptrChar += sizeof(uint16_t);

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to search in
		if(size) {
			// Check if the current character is the wanted character
			if(*ptrChar == (char_t)c)
				return ptrChar;
		}

		return nullptr;
	}
	const void* memchr(const void* ptr, int32_t c, size_t size) {
		// Convert the given pointer to a char pointer
		const char_t* ptrChar = (const char_t*)ptr;

		// Search for the character in size_t segments
		while(size >= sizeof(size_t)) {
			// Save a size_t segment
			size_t segment = *(const size_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the character in the given segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Increment the pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of size_t
			ptrChar += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to search in
		if(size >= sizeof(uint32_t)) {
			// Save an uint32_t segment
			uint32_t segment = *(const uint32_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the character in the given segment
			for(size_t i = 0; i != sizeof(uint32_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Increment the pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of uint32_t
			ptrChar += sizeof(uint32_t);

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to search in
		if(size >= sizeof(uint16_t)) {
			// Save an uint16_t segment
			uint16_t segment = *(const uint16_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the character in the given segment
			for(size_t i = 0; i != sizeof(uint16_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Increment the pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of uint16_t
			ptrChar += sizeof(uint16_t);

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to search in
		if(size) {
			// Check if the current character is the wanted character
			if(*ptrChar == (char_t)c)
				return ptrChar;
		}

		return nullptr;
	}
	void* rawmemchr(void* ptr, int32_t c) {
		// Convert the given pointer to a char pointer
		char_t* ptrChar = (char_t*)ptr;

		// Search for the character in size_t segments
		do {
			// Save a size_t segment
			size_t segment = *(size_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the character in the given segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of size_t
			ptrChar += sizeof(size_t);            
		} while(ptrChar != ptr);

		// To avoid compiler error
		return nullptr;
	}
	const void* rawmemchr(const void* ptr, int32_t c) {
		// Convert the given pointer to a char pointer
		const char_t* ptrChar = (const char_t*)ptr;

		// Search for the character in size_t segments
		do {
			// Save a size_t segment
			size_t segment = *(const size_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the character in the given segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the char pointer by the size of size_t
			ptrChar += sizeof(size_t);            
		} while(ptrChar != ptr);

		// To avoid compiler error
		return nullptr;
	}
	void* memrchr(void* ptr, int32_t c, size_t size) {
		// Convert the given pointer to a char pointer
		char_t* ptrChar = (char_t*)ptr + size;

		// Search for the character in size_t segments
		while(size >= sizeof(size_t)) {
			// Decrement the char pointer by the size of size_t
			ptrChar -= sizeof(size_t);
			
			// Save a size_t segment
			size_t segment = *(size_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment + sizeof(size_t) - 1;
			for(size_t i = sizeof(size_t) - 1; i != SIZE_T_MAX; --i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Decrement the pointer
				--segmentPtr;
			}

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to search in
		if(size >= sizeof(uint32_t)) {
			// Decrement the char pointer by the size of uint32_t
			ptrChar -= sizeof(uint32_t);

			// Save an uint32_t segment
			uint32_t segment = *(uint32_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment + sizeof(uint32_t) - 1;
			for(size_t i = sizeof(uint32_t) - 1; i != SIZE_T_MAX; --i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Decrement the pointer
				--segmentPtr;
			}

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to search in
		if(size >= sizeof(uint16_t)) {
			// Decrement the char pointer by the size of uint16_t
			ptrChar -= sizeof(uint16_t);

			// Save an uint16_t segment
			uint16_t segment = *(uint16_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment + sizeof(uint16_t) - 1;
			for(size_t i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; --i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + i;

				// Decrement the pointer
				--segmentPtr;
			}

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to search in
		if(size) {
			// Decrement the char pointer by the size of uint8_t
			ptrChar -= sizeof(uint8_t);

			// Check if the current character is the wanted character
			if(*ptrChar == (char_t)c)
				return ptrChar;
		}

		return nullptr;
	}
	const void* memrchr(const void* ptr, int32_t c, size_t size) {
		// Convert the given pointer to a char pointer
		const char_t* ptrChar = (const char_t*)ptr + size;

		// Search for the character in size_t segments
		while(size >= sizeof(size_t)) {
			// Decrement the char pointer by the size of size_t
			ptrChar -= sizeof(size_t);
			
			// Save a size_t segment
			size_t segment = *(const size_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment + sizeof(size_t) - 1;

			for(size_t i = sizeof(size_t) - 1; i != SIZE_T_MAX; ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + sizeof(size_t) - i - 1;

				// Decrement the pointer
				--segmentPtr;
			}

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to search in
		if(size >= sizeof(uint32_t)) {
			// Decrement the char pointer by the size of uint32_t
			ptrChar -= sizeof(uint32_t);

			// Save an uint32_t segment
			uint32_t segment = *(const uint32_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment + sizeof(uint32_t) - 1;
			for(size_t i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + sizeof(uint32_t) - i - 1;

				// Decrement the pointer
				--segmentPtr;
			}

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to search in
		if(size >= sizeof(uint16_t)) {
			// Decrement the char pointer by the size of uint16_t
			ptrChar -= sizeof(uint16_t);

			// Save an uint16_t segment
			uint16_t segment = *(const uint16_t*)ptrChar;

			const char_t* segmentPtr = (const char_t*)&segment + sizeof(uint16_t) - 1;
			for(size_t i = sizeof(uint16_t) - 1; i != SIZE_T_MAX; ++i) {
				// Check if the wanted character is in this segment
				if(*segmentPtr == (char_t)c)
					return ptrChar + sizeof(uint16_t) - i - 1;

				// Decrement the pointer
				--segmentPtr;
			}

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to search in
		if(size) {
			// Decrement the char pointer by the size of uint8_t
			ptrChar -= sizeof(uint8_t);

			// Check if the current character is the wanted character
			if(*ptrChar == (char_t)c)
				return ptrChar;
		}

		return nullptr;
	}

	char_t* strcpy(char_t* dest, const char_t* src) {
		// Exit if the target and source string block are the same; nothing needs to be copied
		if(dest == src)
			return dest;
		
		// Save the original target string for the return value
		char_t* originalDest = dest;

		// Save a leftover size to be copied for then the null termination character is found
		size_t leftoverSize = 0;

		// Copy everything in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(size_t*)src;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the null termination character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				if(!*segmentPtr) {
					leftoverSize = i + 1;
					if(leftoverSize == sizeof(size_t)) {
						// Copy one size_t segment
						*(size_t*)dest = segment;

						// Exit the function
						return originalDest;
					}
					break;
				}

				// Increment the segment pointer
				++segmentPtr;
			}
			// Exit if the null termination character was found
			if(leftoverSize)
				break;

			// Copy one size_t segment
			*(size_t*)dest = segment;

			// Increment the pointers by the size of size_t
			dest += sizeof(size_t);
			src += sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to copy
		if(leftoverSize >= sizeof(uint32_t)) {
			// Copy one uint32_t segment
			*(uint32_t*)dest = *(uint32_t*)src;

			// Increment the pointers by the size of uint32_t
			dest += sizeof(uint32_t);
			src += sizeof(uint32_t);

			// Decrement the size by the size of uint32_t
			leftoverSize -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to copy
		if(leftoverSize >= sizeof(uint16_t)) {
			// Copy one uint16_t segment
			*(uint16_t*)dest= *(uint16_t*)src;

			// Increment the pointers by the size of uint16_t
			dest += sizeof(uint16_t);
			src += sizeof(uint16_t);

			// Decrement the size by the size of uint16_t
			leftoverSize -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to copy
		if(leftoverSize) {
			// Copy one uint8_t segment
			*(uint8_t*)dest = *(uint8_t*)src;
		}

		return originalDest;
	}
	char_t* strncpy(char_t* dest, const char_t* src, size_t size) {
		// Copy using memccpy
		memccpy(dest, src, 0, size);
		return dest;
	}
	char_t* strcat(char_t* dest, const char_t* src) {
		// Copy the source string at the end of the destination string
		return strcpy(dest + strlen(dest), src);
	}
	char_t* strncat(char_t* dest, const char_t* src, size_t size) {
		// Copy the source string at the end of the destination string
		return strncpy(dest + strlen(dest), src, size);
	}

	int32_t strcmp(const char_t* str1, const char_t* str2) {
		// Exit if the strings are the same; nothing needs to be compared
		if(str1 == str2)
			return 0;
		
		// Compare the strings in segments of size_t
		while(true) {
			// Save two size_t segments
			size_t segment1 = *(const size_t*)str1;
			size_t segment2 = *(const size_t*)str2;

			const char_t* segment1Ptr = (const char_t*)&segment1;
			const char_t* segment2Ptr = (const char_t*)&segment2;

			// Compare all characters
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Exit the loop if the null termination character was found
				if(!*segment1Ptr || !*segment2Ptr) {
					// Move thw two strings up to the null termination character
					str1 += i;
					str2 += i;

					break;
				}

				// Compare the two characters
				if(*segment1Ptr > *segment2Ptr)
					return 1;
				else if(*segment1Ptr < *segment2Ptr)
					return -1;
				
				// Increment the segment pointers
				++segment1Ptr;
				++segment2Ptr;
			}
			// Exit the loop if the null termination character was found
			if(!*str1 || !*str2)
				break;
			
			// Increment the string pointers by the size of size_t
			str1 += sizeof(size_t);
			str2 += sizeof(size_t);
		}

		// Both string are equal up to the first end; check which one is longer
		if(*str1)
			return 1;
		else if(*str2)
			return -1;
		
		// Both string are equal and have the same length; return 0
		return 0;
	}
	int32_t strncmp(const char_t* str1, const char_t* str2, size_t size) {
		// Exit if the strings are the same; nothing needs to be compared
		if(str1 == str2)
			return 0;
		
		// Save the original pointers
		const char_t* oldStr1 = str1;
		size_t oldSize = size;
		
		// Compare the strings in segments of size_t
		while(size >= sizeof(size_t)) {
			// Save two size_t segments
			size_t segment1 = *(const size_t*)str1;
			size_t segment2 = *(const size_t*)str2;

			const char_t* segment1Ptr = (const char_t*)&segment1;
			const char_t* segment2Ptr = (const char_t*)&segment2;

			// Compare all characters
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Exit the loop if the null termination character was found
				if(!*segment1Ptr || !*segment2Ptr) {
					// Set the size to zero; nothing else needs to be compared
					size = 0;

					// Move the two strings up to the null termination character
					str1 += i;
					str2 += i;

					break;
				}

				// Compare both characters
				if(*segment1Ptr > *segment2Ptr)
					return 1;
				else if(*segment1Ptr < *segment2Ptr)
					return -1;
				
				// Increment both pointers
				++segment1Ptr;
				++segment2Ptr;
			}
			// Exit the loop if the null termination character was found
			if(!*str1 || !*str2)
				break;

			// Increment both pointers by the size of size_t
			str1 += sizeof(size_t);
			str2 += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to compare
		if(size >= sizeof(uint32_t)) {
			// Save two uint32_t segments
			uint32_t segment1 = *(const uint32_t*)str1;
			uint32_t segment2 = *(const uint32_t*)str1;

			const char_t* segment1Ptr = (const char_t*)&segment1;
			const char_t* segment2Ptr = (const char_t*)&segment2;

			// Compare all characters
			for(size_t i = 0; i != sizeof(uint32_t); ++i) {
				// Exit the loop if the null termination character was found
				if(!*segment1Ptr || !*segment2Ptr) {
					// Set the size to zero; nothing else needs to be compared
					size = 0;

					// Move the two strings up to the null termination character
					str1 += i;
					str2 += i;

					break;
				}

				// Compare both characters
				if(*segment1Ptr < *segment2Ptr)
					return -1;
				else if(*segment1Ptr > *segment2Ptr)
					return 1;

				// Increment both pointers
				++segment1Ptr;
				++segment2Ptr;
			}
			// Check if the null termination character was found
			if(*str1 || *str2) {
				// Increment both pointers by the size of uint32_t
				str1 += sizeof(uint32_t);
				str2 += sizeof(uint32_t);

				// Decrement the size by the size of uint32_t
				size -= sizeof(uint32_t);
			}
		}
#endif
		// Check if there is a 2 byte region left to compare
		if(size >= sizeof(uint16_t)) {
			// Save two uint16_t segments
			uint16_t segment1 = *(const uint32_t*)str1;
			uint16_t segment2 = *(const uint32_t*)str1;

			const char_t* segment1Ptr = (const char_t*)&segment1;
			const char_t* segment2Ptr = (const char_t*)&segment2;

			// Compare all characters
			for(size_t i = 0; i != sizeof(uint16_t); ++i) {
				// Exit the loop if the null termination was found
				if(!*segment1Ptr || !*segment2Ptr) {
					// Set the size to zero; nothing else needs to be compared
					size = 0;

					// Move the two strings up to the null termination character
					str1 += i;
					str2 += i;

					break;
				}

				// Compare both characters
				if(*segment1Ptr < *segment2Ptr)
					return -1;
				else if(*segment1Ptr > *segment2Ptr)
					return 1;

				// Increment both pointers
				++segment1Ptr;
				++segment2Ptr;
			}
			if(!*str1 || !*str2) {
				// Increment both pointers by the size of uint16_t
				str1 += sizeof(uint16_t);
				str2 += sizeof(uint16_t);

				// Decrement the size by the size of uint16_t
				size -= sizeof(uint16_t);
			}
		}
		// Check if there is a 1 byte region left to compare
		if(size) {
			// Save the two current characters
			char_t char1 = *str1, char2 = *str2;

			// Compare the current characters
			if(char1 < char2)
				return -1;
			else if(char1 > char2)
				return 1;
			
			// Increment both pointers by the size of uint8_t
			str1 += sizeof(uint8_t);
			str2 += sizeof(uint8_t);
		}

		// Exit the function if size characters were compared
		if((size_t)(str1 - oldStr1) == oldSize)
			return 0;

		// Both string are equal up to the first end; check which one is longer
		if(*str1 && !*str2)
			return 1;
		else if(*str2 && !*str1)
			return -1;
		
		// Both string are equal and have the same length; return 0
		return 0;
	}
	char_t* strchr(char_t* str, int32_t c) {
		// Look for the wanted character in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the wanted character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr)
					return nullptr;
				
				// Check if the wanted character was found
				if(*segmentPtr == (char_t)c)
					return str + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return nullptr;
	}
	const char_t* strchr(const char_t* str, int32_t c) {
		// Look for the wanted character in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the wanted character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr)
					return nullptr;
				
				// Check if the wanted character was found
				if(*segmentPtr == (char_t)c)
					return str + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return nullptr;
	}
	char_t* strrchr(char_t* str, int32_t c) {
		// Save the last occurence of the character
		char_t* lastOccurence = nullptr;

		// Look for the wanted character in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the wanted character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr) {
					// Move the string pointer up to the null termination character and exit the loop
					str += i;
					break;
				}

				// Check if the wanted character was found
				if(*segmentPtr == (char_t)c)
					lastOccurence = str + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}
			// Exit the loop if the null termination character was found
			if(!*str)
				break;
			
			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// Return the last occurence of the wanted character
		return lastOccurence;
	}
	const char_t* strrchr(const char_t* str, int32_t c) {
		// Save the last occurence of the character
		const char_t* lastOccurence = nullptr;

		// Look for the wanted character in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the wanted character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr) {
					// Move the string pointer up to the null termination character and exit the loop
					str += i;
					break;
				}

				// Check if the wanted character was found
				if(*segmentPtr == (char_t)c)
					lastOccurence = str + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}
			// Exit the loop if the null termination character was found
			if(!*str)
				break;
			
			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// Return the last occurence of the wanted character
		return lastOccurence;
	}
	size_t strcspn(const char_t* str, const char_t* reject) {
		// Create an apprearence vector for every character in reject
		uint8_t appears[32];
		memset(appears, 0, 32 * sizeof(uint8_t));

		// Look through the reject string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)reject;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Add the characters in the segment to the appearence vector
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr) {
					// Move the reject string pointer up to the null termination character and exit the loop
					reject += i;
					break;
				}

				// Add the current character to the appearence vector
				appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)(*segmentPtr) & 7);

				// Increment the segment pointer
				++segmentPtr;
			}
			// Exit the loop if the null termination character was found
			if(!*reject)
				break;
			
			// Increment the reject string pointer by the size of size_t
			reject += sizeof(size_t);
		}

		// Save the number of characters that aren't in reject
		size_t size = 0;

		// Look through the string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Check every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr)
					return size;
				
				// Check if a character fron reject was found
				if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
					return size;
				
				// Increment the size of the segment
				++size;

				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return 0;        
	}
	size_t strspn(const char_t* str, const char_t* accept) {
		// Create an apprearence vector for every character in accept
		uint8_t appears[32];
		memset(appears, 0, 32 * sizeof(uint8_t));

		// Look through the accept string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)accept;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Add the characters in the segment to the appearence vector
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr) {
					// Move the accept string pointer up to the null termination character and exit the loop
					accept += i;
					break;
				}

				// Add the current character to the appearence vector
				appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)(*segmentPtr) & 7);

				// Increment the segment pointer
				++segmentPtr;
			}
			// Exit the loop if the null termination character was found
			if(!*accept)
				break;
			
			// Increment the accept string pointer by the size of size_t
			accept += sizeof(size_t);
		}

		// Save the number of characters that are in accept
		size_t size = 0;

		// Look through the string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Check every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr)
					return size;
				
				// Check if a character fron accept wasn't found
				if(!(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7))))
					return size;
				
				// Increment the size of the segment
				++size;

				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return 0;        
	}
	char_t* strpbrk(char_t* str, const char_t* accept) {
		// Create an appearence vector for every character in accept
		uint8_t appears[32];
		memset(appears, 0, 32 * sizeof(uint8_t));

		// Look through the accept string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)accept;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Add the characters in the segment to the appearence vector
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr) {
					// Move the accept string pointer up to the null termination character and exit the loop
					accept += i;
					break;
				}

				// Add the current character to the appearence vector
				appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)(*segmentPtr) & 7);

				// Increment the segment pointer
				++segmentPtr;
			}
			// Exit the loop if the null termination character was found
			if(!*accept)
				break;
			
			// Increment the accept string pointer by the size of size_t
			accept += sizeof(size_t);
		}

		// Look through the string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Check every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr)
					return nullptr;
				
				// Check if a character fron accept wasn't found
				if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
					return str + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return nullptr;        
	}
	const char_t* strpbrk(const char_t* str, const char_t* accept) {
		// Create an apprearence vector for every character in accept
		uint8_t appears[32];

		// Look through the accept string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)accept;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Add the characters in the segment to the appearence vector
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr) {
					// Move the accept string pointer up to the null termination character and exit the loop
					accept += i;
					break;
				}

				// Add the current character to the appearence vector
				appears[(uint8_t)(*segmentPtr) >> 3] |= 1 << ((uint8_t)*segmentPtr & 7);

				// Increment the segment pointer
				++segmentPtr;
			}
			// Exit the loop if the null termination character was found
			if(!*accept)
				break;
			
			// Increment the accept string pointer by the size of size_t
			accept += sizeof(size_t);
		}

		// Look through the string in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Check every character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the null termination character was found
				if(!*segmentPtr)
					return nullptr;
				
				// Check if a character fron accept wasn't found
				if(appears[(uint8_t)(*segmentPtr) >> 3] & (1 << ((uint8_t)(*segmentPtr) & 7)))
					return str + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return nullptr;        
	}
	char_t* strstr(char_t* str, const char_t* wanted) {
		// Generate the KMP table
		size_t wantedLength = strlen(wanted);

		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		size_t pos = 1;
		ptrdiff_t candidate = 0;
		while(pos < wantedLength) {
			if(wanted[pos] == wanted[candidate])
				table[pos] = table[candidate];
			else {
				table[pos] = candidate;
				while(candidate >= 0 && wanted[pos] != wanted[candidate])
					candidate = table[candidate];
			}

			++pos;
			++candidate;
		}
		table[pos] = candidate;

		// Look for the wanted string in segments of size_t
		pos = 0;

		while(true) {
			// Save a size_t segment
			size_t segment = *(size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the wanted string in the given segment
			for(size_t i = 0; i != sizeof(size_t);) {
				// Exit the function if the null termination character was found
				if(!segmentPtr[i]) {
					free(table, MEMORY_USAGE_ARRAY);
					return nullptr;
				}

				// Check if a matching character was found
				if(wanted[pos] == segmentPtr[i]) {
					// Move on to the next character
					++i;
					++pos;

					// Exit the function if a match for the whole string was found
					if(pos == wantedLength) {
						free(table, MEMORY_USAGE_ARRAY);
						return str + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					ptrdiff_t position = table[pos];
					if(position < 0) {
						++i;
						pos = 0;
					} else
						pos = (size_t)position;
				}
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return nullptr;
	}
	const char_t* strstr(const char_t* str, const char_t* wanted) {
		// Generate the KMP table
		size_t wantedLength = strlen(wanted);

		ptrdiff_t* table = (ptrdiff_t*)calloc(wantedLength + 1, sizeof(ptrdiff_t), MEMORY_USAGE_ARRAY);
		table[0] = -1;

		size_t pos = 1;
		ptrdiff_t candidate = 0;
		while(pos < wantedLength) {
			if(wanted[pos] == wanted[candidate])
				table[pos] = table[candidate];
			else {
				table[pos] = candidate;
				while(candidate >= 0 && wanted[pos] != wanted[candidate])
					candidate = table[candidate];
			}

			++pos;
			++candidate;
		}
		table[pos] = candidate;

		// Look for the wanted string in segments of size_t
		pos = 0;

		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the wanted string in the given segment
			for(size_t i = 0; i != sizeof(size_t);) {
				// Exit the function if the null termination character was found
				if(!segmentPtr[i]) {
					free(table, MEMORY_USAGE_ARRAY);
					return nullptr;
				}

				// Check if a matching character was found
				if(wanted[pos] == segmentPtr[i]) {
					// Move on to the next character
					++i;
					++pos;

					// Exit the character if a match for the whole string was found
					if(pos == wantedLength) {
						free(table, MEMORY_USAGE_ARRAY);
						return str + i - wantedLength;
					}
				} else {
					// Start over at the highest match possible
					ptrdiff_t position = table[pos];
					if(position < 0) {
						++i;
						pos = 0;
					} else
						pos = (size_t)position;
				}
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);
		}

		// To avoid compiler error
		return nullptr;
	}

	char_t* strtok(char_t* str, const char_t* delim) {
		static char_t* lastStr;

		// Set the wanted string to the last string if none was specified
		if(!str)
			str = lastStr;

		// Exit the function if there is no token to look for
		if(!*str)
			return nullptr;
		
		// Remove all starting delim characters from the string
		str += strspn(str, delim);

		// Move the last string up to the next delim characters
		lastStr = str + strcspn(str, delim);

		// Move the last string up to the next token
		char_t* lastPos = lastStr;
		lastStr += strspn(lastStr, delim);

		// Put a null termination character at the end of the token
		*lastPos = 0;

		// Return the current token
		return str;
	}

	size_t strlen(const char_t* str) {
		size_t size = 0;

		// Look for the null termination character in segments of size_t
		while(true) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the null termination character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the current character is the null termination character
				if(!*segmentPtr)
					return size + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);

			// Increment the size by the size of size_t
			size += sizeof(size_t);
		}

		// To avoid compiler error
		return 0;
	}
	size_t strnlen(const char_t* str, size_t size) {
		size_t stringSize = 0;

		// Look for the null termination character in segments of size_t
		while(size >= sizeof(size_t)) {
			// Save a size_t segment
			size_t segment = *(const size_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the null termination character in the segment
			for(size_t i = 0; i != sizeof(size_t); ++i) {
				// Check if the current character is the null termination character
				if(!*segmentPtr)
					return stringSize + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of size_t
			str += sizeof(size_t);

			// Increment the string size by the size of size_t
			stringSize += sizeof(size_t);

			// Decrement the size by the size of size_t
			size -= sizeof(size_t);
		}

#if defined(WFE_ARCHITECTURE_X64)
		// Check if there is a 4 byte region left to search in
		if(size >= sizeof(uint32_t)) {
			// Save an uint32_t segment
			uint32_t segment = *(const uint32_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the null termination character in the segment
			for(size_t i = 0; i != sizeof(uint32_t); ++i) {
				// Check if the current character is the null termination character
				if(!*segmentPtr)
					return stringSize + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of uint32_t
			str += sizeof(uint32_t);

			// Increment the string size by the size of uint32_t
			stringSize += sizeof(uint32_t);

			// Decrement the size by the size of uint32_t
			size -= sizeof(uint32_t);
		}
#endif
		// Check if there is a 2 byte region left to search in
		if(size >= sizeof(uint16_t)) {
			// Save an uint16_t segment
			uint16_t segment = *(const uint16_t*)str;

			const char_t* segmentPtr = (const char_t*)&segment;

			// Look for the null termination character in the segment
			for(size_t i = 0; i != sizeof(uint16_t); ++i) {
				// Check if the current character is the null termination character
				if(!*segmentPtr)
					return stringSize + i;
				
				// Increment the segment pointer
				++segmentPtr;
			}

			// Increment the string pointer by the size of uint16_t
			str += sizeof(uint16_t);

			// Increment the string size by the size of uint16_t
			stringSize += sizeof(uint16_t);

			// Decrement the size by the size of uint16_t
			size -= sizeof(uint16_t);
		}
		// Check if there is a 1 byte region left to search in
		if(size) {
			// Check if the current character is the null termination character
			if(!*str)
				return stringSize;
			
			++stringSize;
		}

		// No null termination character was found; return the given max size
		return stringSize;
	}
}