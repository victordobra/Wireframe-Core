#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX

#include "Files.hpp"
#include "Memory.hpp"

#include <stdio.h>

namespace wfe {
	const size_t FILE_READ_BUFFER_SIZE = 8192;
	const char_t* NUM_ACCEPT_STRING = "0123456789";

	const size_t FILE_WRITE_MAX_BUFFER_SIZE = 65536;

	FileInput::FileInput(const string& filePath, StreamType streamType) {
		// Open the file input stream using the given parameters
		Open(filePath, streamType);
	}

	bool8_t FileInput::Open(const string& filePath, StreamType streamType) {
		// Exit the function if the stream is already open
		if(internalData)
			return false;

        // Set the file modes based on the given stream type flags
        const char_t* modes;
        if(streamType & STREAM_TYPE_BINARY)
            modes = "rb";
        else
            modes = "r";

		// Open the file using fopen
		internalData = fopen(filePath.c_str(), modes);

		// Check if the function succeeded
		if(!internalData)
			return false;
		
		// Check if the STREAM_TYPE_AT_THE_END flag is set
		if(streamType & STREAM_TYPE_AT_THE_END) {
			// Move the file pointer at the end of the file
			int32_t result = fseek((FILE*)internalData, 0, SEEK_END);

			// Check if the cursor was moved successfully
			return !result;
		}

		return true;
	}
	bool8_t FileInput::Close() {
		// Exit the function if the stream is already closed
		if(!internalData)
			return false;

		// Close the output stream using fclose
		int32_t result = fclose((FILE*)internalData);

		// Set the internal data pointer to 0 if the stream was closed successfully
		if(result != EOF)
			internalData = nullptr;
		
		return result != EOF;
	}

	char_t FileInput::Get() {
		// Return a null termination character if the stream is not open
		if(!internalData)
			return 0;

        // Read the following character from the stream
		int32_t character = fgetc((FILE*)internalData);

        // Check if a character was read
        if(character == EOF)
            return 0;
		
		// Return the character
		return (char_t)character;
	}
	size_t FileInput::ReadBuffer(size_t size, void* buffer) {
		// Return a 0 if the stream is not open
		if(!internalData)
			return 0;
		
		// Read the requested buffer from the file
		size_t readCount = fread(buffer, 1, size, (FILE*)internalData);

		return readCount;
	}

	size_t FileInput::Read(size_t valCount, int8_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		int8_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
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

            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
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

            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
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
			
            size_t bufferSize = fread(buffer, 1, FILE_READ_BUFFER_SIZE, (FILE*)internalData);

			// Exit the loop if the read failed
			if(!bufferSize)
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
		// Get the cursor's position using ftell
		return (uint64_t)ftell((FILE*)internalData);
	}
	bool8_t FileInput::SetPos(int64_t pos, FileInput::SetPosRelative setRel) {
		// Set the Linux whence based on the given set pos relative
		int32_t whence;
		switch(setRel) {
		case SET_POS_RELATIVE_BEGIN:
			whence = SEEK_SET;
			break;
		case SET_POS_RELATIVE_END:
			whence = SEEK_END;
			break;
		case SET_POS_RELATIVE_POS:
			whence = SEEK_CUR;
			break;
		}

		// Set the cursor's position
		int32_t result = fseek((FILE*)internalData, (long)pos, whence);
	
		// Check if the function succeeded
		return !result;
	}

	bool8_t FileInput::IsOpen() const {
		// The file is open if its internal data is not set to nullptr
		return internalData;
	}
	bool8_t FileInput::IsAtTheEnd() const {
		// Check if the file is at the end using feof
		return feof((FILE*)internalData);
	}
	uint64_t FileInput::GetSize() const {
		// Save the stream's current position
        uint64_t pos = GetPos();

        // Move the file cursor at the end
        fseek((FILE*)internalData, 0, SEEK_END);

        // Now the stream's position is the file's size; read it
        uint64_t size = GetPos();

        // Move the file cursor back to its original position
        fseek((FILE*)internalData, (long)pos, SEEK_SET);

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
		
        // Set the file modes base on the given stream type flags
        char_t fileModes[3];

        if(streamType & STREAM_TYPE_APPEND)
            fileModes[0] = 'a';
        else
            fileModes[0] = 'w';
        
        if(streamType & STREAM_TYPE_BINARY) {
            fileModes[1] = 'b';
            fileModes[2] = 0;
        } else
            fileModes[1] = 0;
        
        // Open the file output stream using fopen
        internalData = fopen(filePath.c_str(), fileModes);

		return internalData;
	}
	bool8_t FileOutput::Close() {
		// Exit the function if the stream is already closed
		if(!internalData)
			return false;

		// Close the file output stream using fclose
        int32_t result = fclose((FILE*)internalData);

		// Set the internal data pointer to 0 if the stream was closed successfully
        if(result != EOF)
            internalData = nullptr;
		
		return result != EOF;
	}

	size_t FileOutput::WriteBuffer(size_t size, const void* buffer) {
		// Return a 0 if the stream is not open
		if(!internalData)
			return 0;

		// Read the requested buffer
		size_t writeCount = fwrite(buffer, 1, size, (FILE*)internalData);
		
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
			size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

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
		    size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = fwrite(output.data(), 1, output.length(), (FILE*)internalData);

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}

	bool8_t FileOutput::Flush() {
		// Flush the file's changes
		return !fflush((FILE*)internalData);
	}

	uint64_t FileOutput::GetPos() const {
        // Get the pointer's current position using ftell
		return (uint64_t)ftell((FILE*)internalData);
	}
	bool8_t FileOutput::SetPos(int64_t pos, FileOutput::SetPosRelative setRel) {
		// Set the Linux whence based on the given set pos relative
		int32_t whence;
		switch(setRel) {
		case SET_POS_RELATIVE_BEGIN:
			whence = SEEK_SET;
			break;
		case SET_POS_RELATIVE_END:
			whence = SEEK_END;
			break;
		case SET_POS_RELATIVE_POS:
			whence = SEEK_CUR;
			break;
		}

		// Set the cursor's position
		int32_t result = fseek((FILE*)internalData, (long)pos, whence);
	
		// Check if the function succeeded
		return !result;
	}

	bool8_t FileOutput::IsOpen() const {
		// The file is open if its internal data is not set to nullptr
		return internalData;
	}
	bool8_t FileOutput::IsAtTheEnd() const {
		// Check if the file is at the end using feof
		return feof((FILE*)internalData);
	}
	uint64_t FileOutput::GetSize() const {
		// Save the stream's current position
        uint64_t pos = GetPos();

        // Move the file cursor at the end
        fseek((FILE*)internalData, 0, SEEK_END);

        // Now the stream's position is the file's size; read it
        uint64_t size = GetPos();

        // Move the file cursor back to its original position
        fseek((FILE*)internalData, (long)pos, SEEK_SET);

		return size;
	}

	FileOutput::~FileOutput() {
		// Close the file output stream
		Close();
	}
}

#endif