#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Files.hpp"
#include "Memory.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
	static const size_t FILE_READ_BUFFER_SIZE = 8192;
	static const char_t* NUM_ACCEPT_STRING = "0123456789";

	static const size_t FILE_WRITE_MAX_BUFFER_SIZE = 65536;

	FileInput::FileInput(const string& filePath, StreamType streamType) {
		// Open the file input stream using the given parameters
		Open(filePath, streamType);
	}

	bool8_t FileInput::Open(const string& filePath, StreamType streamType) {
		// Exit the function if the stream is already open
		if(internalData)
			return false;

		// Open the file using CreateFile
		internalData = (void*)CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_VALID_FLAGS, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		// Check if the function succeeded
		if(internalData == INVALID_HANDLE_VALUE)
			return false;
		
		// Check if the STREAM_TYPE_AT_THE_END flag is set
		if(streamType & STREAM_TYPE_AT_THE_END) {
			// Move the file pointer at the end of the file
			DWORD result = SetFilePointer((HANDLE)internalData, 0, nullptr, FILE_END);

			// Check if the cursor was moved successfully
			return (result != ERROR_NEGATIVE_SEEK) && (result != INVALID_SET_FILE_POINTER);
		}

		return true;
	}
	bool8_t FileInput::Close() {
		// Exit the function if the stream is already closed
		if(!internalData)
			return false;

		// Simply close the handle of the file input stream
		bool8_t result = CloseHandle((HANDLE)internalData);

		// Set the internal data pointer to 0 if the stream was closed successfully
		if(result)
			internalData = nullptr;
		
		return result;
	}

	char_t FileInput::Get() {
		// Return a null termination character if the stream is not open
		if(!internalData)
			return 0;

		// Read just one character from the file
		char_t character;
		DWORD charRead;

		bool8_t result = ReadFile((HANDLE)internalData, &character, 1, &charRead, nullptr);

		// Check if the character was read successfully
		if(!charRead || !result)
			character = 0;
		
		// Return the character
		return character;
	}
	size_t FileInput::ReadBuffer(size_t size, void* buffer) {
		// Return a 0 if the stream is not open
		if(!internalData)
			return 0;
		
		// Read the requested buffer from the file
		DWORD readSize;

		bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)size, &readSize, nullptr);

		// Check if the buffer was read successfully
		if(!result)
			readSize = 0;
		
		return readSize;
	}

	size_t FileInput::Read(size_t valCount, int8_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		int8_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0)
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				else
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
			}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				int8_t val = (int8_t)StrToInt(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Invert the value if the previous character is a minus
				if(ptr != buffer && ptr[-1] == '-')
					val = -val;
				
				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, int16_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		int16_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0)
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				else
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
			}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				int16_t val = (int16_t)StrToInt(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Invert the value if the previous character is a minus
				if(ptr != buffer && ptr[-1] == '-')
					val = -val;
				
				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, int32_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		int32_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0)
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				else
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
			}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				int32_t val = (int32_t)StrToInt(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Invert the value if the previous character is a minus
				if(ptr != buffer && ptr[-1] == '-')
					val = -val;
				
				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, int64_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		int64_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0)
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				else
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
			}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				int64_t val = (int64_t)StrToInt(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Invert the value if the previous character is a minus
				if(ptr != buffer && ptr[-1] == '-')
					val = -val;
				
				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, uint8_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		uint8_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
				while(*ptr >= '0' && *ptr <= '9') {
					// Add the current digit to the last value
					lastVal *= 10;
					lastVal += *ptr - '0';

					// Increment the pointer
					++ptr;
				}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				uint8_t val = (uint8_t)StrToUint(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, uint16_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		uint16_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
				while(*ptr >= '0' && *ptr <= '9') {
					// Add the current digit to the last value
					lastVal *= 10;
					lastVal += *ptr - '0';

					// Increment the pointer
					++ptr;
				}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				uint16_t val = (uint16_t)StrToUint(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, uint32_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		uint32_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
				while(*ptr >= '0' && *ptr <= '9') {
					// Add the current digit to the last value
					lastVal *= 10;
					lastVal += *ptr - '0';

					// Increment the pointer
					++ptr;
				}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				uint32_t val = (uint32_t)StrToUint(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, uint64_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		uint64_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
				while(*ptr >= '0' && *ptr <= '9') {
					// Add the current digit to the last value
					lastVal *= 10;
					lastVal += *ptr - '0';

					// Increment the pointer
					++ptr;
				}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				uint64_t val = (uint64_t)StrToUint(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, float32_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		float32_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
				while(*ptr >= '0' && *ptr <= '9') {
					// Add the current digit to the last value
					lastVal *= 10;
					lastVal += *ptr - '0';

					// Increment the pointer
					++ptr;
				}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				float32_t val = StrToFloat(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t valCount, float64_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		float64_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
				while(*ptr >= '0' && *ptr <= '9') {
					// Add the current digit to the last value
					lastVal *= 10;
					lastVal += *ptr - '0';

					// Increment the pointer
					++ptr;
				}

			// Check if there are no following characters. If so, move on to the next buffer
			if(!*ptr)
				continue;
			
			// Check if the last value was set
			if(lastValSet) {
				// Add the last value to the array and reset it
				*vals = lastVal;
				++readCount;
				++vals;

				lastVal = 0;
				lastValSet = false;
			}
			
			// Look for the first numerical value in the buffer
			ptr = strpbrk(ptr, NUM_ACCEPT_STRING);

			while(ptr && readCount != valCount) {
				// Read the current value
				size_t index;
				float64_t val = StrToDouble(ptr, &index);

				// Exit the loop if no value was found
				if(!index)
					break;

				// Set the new pointer's value
				ptr += index;

				// Check if the current value is the last value from the buffer
				if(!*ptr) {
					// Don't write the value to the value array, instead write it to the lastVal variable and store it for the next buffer
					lastVal = val;
					lastValSet = true;

					break;
				}

				// Write this value to the value array
				*vals = val;
				++readCount;
				++vals;

				// Look for the next numerical value in the buffer
				ptr = strpbrk(ptr, NUM_ACCEPT_STRING);
			}
		}

		// Write the last value to the array if it hasn't already been inserted
		if(readCount != valCount && lastValSet) {
			*vals = lastVal;
			++readCount;
		}

		return readCount;
	}
	size_t FileInput::Read(size_t strCount, string* strings, const char_t* sep) {
		// Keep reading buffers until all requested strings have been read
		size_t readCount = 0;
		bool8_t lastStrSet = false;

		while(readCount != strCount || lastStrSet) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			DWORD bufferSize;

			bool8_t result = ReadFile((HANDLE)internalData, buffer, (DWORD)FILE_READ_BUFFER_SIZE, &bufferSize, nullptr);

			// Exit the loop if the read failed
			if(!result || !bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last string could be continued
			char_t* ptr = buffer;
			if(lastStrSet) {
				// Calculate the number of characters that could be appended to the last string
				size_t spanLength = strcspn(ptr, sep);

				// Append the given characters to the last string
				strings->append(ptr, spanLength);

				// Set the pointer's new value
				ptr += spanLength;

				// Check if this string couldn't be continued
				if(*ptr) {
					// Move on to the next string
					++strings;
					lastStrSet = false;
				}
			}

			// Tokenize the buffer to find the wanted strings
			ptr = strtok(ptr, sep);

			// Find every string from the current buffer
			while(ptr && *ptr) {
				// Copy the current string
				*strings = ptr;
				++readCount;

				// Check if the current string could be continued
				if(ptr + strings->length() == buffer + bufferSize) {
					// Set the lastStrSet variable to true
					lastStrSet = true;
				} else {
					// Increment the string array pointer
					++strings;
				}

				// Find the next string
				ptr = strtok(nullptr, sep);
			}
		}

		return readCount;
	}

	uint64_t FileInput::GetPos() const {
		// Get the cursor's position by using SetFilePointer without modifying its position, via its return value
		LONG highPos = 0;

		DWORD lowPos = SetFilePointer((HANDLE)internalData, 0, &highPos, FILE_CURRENT);

		// Merge the two values into one
		uint64_t pos = (uint64_t)lowPos | ((uint64_t)highPos << 32);

		return pos;
	}
	bool8_t FileInput::SetPos(int64_t pos, FileInput::SetPosRelative setRel) {
		// Set the Win32 move method based on the set pos relative
		DWORD moveMethod;
		switch(setRel) {
		case SET_POS_RELATIVE_BEGIN:
			moveMethod = FILE_BEGIN;
			break;
		case SET_POS_RELATIVE_END:
			moveMethod = FILE_END;
			break;
		case SET_POS_RELATIVE_POS:
			moveMethod = FILE_CURRENT;
			break;
		default:
			moveMethod = FILE_BEGIN;
			break;
		}

		// Split the given position in its low and high components
		LONG lowPos = (LONG)(pos & (((uint64_t)1 << 32) - 1));
		LONG highPos = (LONG)(pos >> 32);

		// Set the cursor's position
		DWORD result = SetFilePointer((HANDLE)internalData, lowPos, &highPos, moveMethod);
	
		// Check if the function succeeded
		return result != INVALID_SET_FILE_POINTER;
	}

	bool8_t FileInput::IsOpen() const {
		// The file is open if its internal data is not set to nullptr
		return internalData;
	}
	bool8_t FileInput::IsAtTheEnd() const {
		// Try to read a character from the file
		char_t buffer;
		DWORD readCount;

		WINBOOL result = ReadFile((HANDLE)internalData, &buffer, 1, &readCount, nullptr);

		// Check if the wanted character was read
		if(result && !readCount)
			return true;
		
		// Move the stream pointer back
		SetFilePointer((HANDLE)internalData, -readCount, nullptr, FILE_CURRENT);

		return false;
	}
	uint64_t FileInput::GetSize() const {
		// Read the low and high components of the file's size
		DWORD highSize;

		DWORD lowSize = GetFileSize((HANDLE)internalData, &highSize);

		// Merge the two components
		uint64_t size = (uint64_t)lowSize | ((uint64_t)highSize << 32);

		return size;
	}

	FileInput::~FileInput() {
		// Close the stream
		Close();
	}

	FileOutput::FileOutput(const string& filePath, StreamType streamType) {
		// Open the output stream using the given parameters
		Open(filePath, streamType);
	}
	
	bool8_t FileOutput::Open(const string& filePath, StreamType streamType) {
		// Exit the fucntion if the stream is already open
		if(internalData)
			return false;
		
		// Try to truncate the file's contents if the STREAM_TYPE_APPEND flag is not set
		if(!(streamType & STREAM_TYPE_APPEND)) {
			// Create the file output stream
			internalData = (void*)CreateFileA(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_VALID_FLAGS, nullptr, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

			// Exit the function if the stream was created successfully
			if(internalData != INVALID_HANDLE_VALUE)
				return true;
			
			// Exit the function if the stream creation failed due to reasons other than the file not existing
			if(GetLastError() != ERROR_FILE_NOT_FOUND)
				return false;
		}

		// Create the file output stream
		internalData = (void*)CreateFileA(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_VALID_FLAGS, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		// Check if the stream was created successfully
		if(internalData == INVALID_HANDLE_VALUE)
			return false;

		// Move the pointer to the end of the file if the append flag is set
		if(streamType & STREAM_TYPE_APPEND) {
			// Move the file pointer at the end of the file
			DWORD result = SetFilePointer((HANDLE)internalData, 0, nullptr, FILE_END);

			// Check if the cursor was moved successfully
			return (result != ERROR_NEGATIVE_SEEK) && (result != INVALID_SET_FILE_POINTER);
		}

		return true;
	}
	bool8_t FileOutput::Close() {
		// Exit the function if the stream is already closed
		if(!internalData)
			return false;

		// Close the stream's handle
		bool8_t result = CloseHandle((HANDLE)internalData);

		// Reset the stream handle if the handle was closed successfully
		if(result)
			internalData = nullptr;
		
		return result;
	}

	size_t FileOutput::WriteBuffer(size_t size, const void* buffer) {
		// Return a 0 if the stream is not open
		if(!internalData)
			return 0;

		// Read the requested buffer
		DWORD writeCount;

		WINBOOL result = WriteFile((HANDLE)internalData, buffer, size, &writeCount, nullptr);

		// Check if the buffer was wirtten successfully
		if(!result)
			writeCount = 0;
		
		return writeCount;
	}

	size_t FileOutput::Write(size_t valCount, const int8_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString((int64_t)vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const int16_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString((int64_t)vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const int32_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString((int64_t)vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const int64_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString(vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const uint8_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString((uint64_t)vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const uint16_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString((uint64_t)vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const uint32_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString((uint64_t)vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const uint64_t* vals, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString(vals[i]));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const float32_t* vals, size_t precision, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString(vals[i], precision));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t valCount, const float64_t* vals, size_t precision, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given values
		size_t writeCount = 0;
		for(size_t i = 0; i != valCount; ++i) {
			// Append the current value to the string
			output.append(ToString(vals[i], precision));

			// Append the separation character to the string
			if(i != valCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t strCount, const string* strings, const char_t* sep) {
		// Create a string containing the total output from the current operation
		string output = "";

		// Precalculate the separation string's length to avoid extra processing
		size_t sepLength = strlen(sep);

		// Loop through the given strings
		size_t writeCount = 0;
		for(size_t i = 0; i != strCount; ++i) {
			// Append the current string to the output string
			output.append(strings[i]);

			// Append the separation character to the output string
			if(i != strCount - 1)
				output.append(sep, sepLength);
			
			// Check if the string's size surpassed the max buffer output size
			if(output.length() < FILE_WRITE_MAX_BUFFER_SIZE)
				continue;
			
			// Write the output's contents to the file
			DWORD fileWriteCount;

			WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(!result || fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		DWORD fileWriteCount;

		WINBOOL result = WriteFile((HANDLE)internalData, output.data(), output.length(), &fileWriteCount, nullptr);

		// Check if the write was successful
		if(!result || fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}

	bool8_t FileOutput::Flush() {
		// Flush the file's changes
		return FlushFileBuffers((HANDLE)internalData);
	}

	uint64_t FileOutput::GetPos() const {
		// Get the cursor's position by using SetFilePointer without modifying its position, via its return value
		LONG highPos = 0;

		DWORD lowPos = SetFilePointer((HANDLE)internalData, 0, &highPos, FILE_CURRENT);

		// Merge the two values into one
		uint64_t pos = (uint64_t)lowPos | ((uint64_t)highPos << 32);

		return pos;
	}
	bool8_t FileOutput::SetPos(int64_t pos, FileOutput::SetPosRelative setRel) {
		// Set the Win32 move method based on the set pos relative
		DWORD moveMethod;
		switch(setRel) {
		case SET_POS_RELATIVE_BEGIN:
			moveMethod = FILE_BEGIN;
			break;
		case SET_POS_RELATIVE_END:
			moveMethod = FILE_END;
			break;
		case SET_POS_RELATIVE_POS:
			moveMethod = FILE_CURRENT;
			break;
		default:
			moveMethod = FILE_BEGIN;
			break;
		}

		// Split the given position in its low and high components
		LONG lowPos = (LONG)(pos & (((uint64_t)1 << 32) - 1));
		LONG highPos = (LONG)(pos >> 32);

		// Set the cursor's position
		DWORD result = SetFilePointer((HANDLE)internalData, lowPos, &highPos, moveMethod);
	
		// Check if the function succeeded
		return result != INVALID_SET_FILE_POINTER;
	}

	bool8_t FileOutput::IsOpen() const {
		// The file is open if its internal data is not set to nullptr
		return internalData;
	}
	bool8_t FileOutput::IsAtTheEnd() const {
		// Check if the cursor's position is equal to the file size
		return GetPos() == GetSize();
	}
	uint64_t FileOutput::GetSize() const {
		// Read the low and high components of the file's size
		DWORD highSize;

		DWORD lowSize = GetFileSize((HANDLE)internalData, &highSize);

		// Merge the two components
		uint64_t size = (uint64_t)lowSize | ((uint64_t)highSize << 32);

		return size;
	}

	FileOutput::~FileOutput() {
		// Close the file output stream
		Close();
	}
}

#endif