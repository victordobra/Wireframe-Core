#include "Files.hpp"
#include "Memory.hpp"

namespace wfe {
	// Constants
	static const size_t FILE_READ_BUFFER_SIZE = 8192;
	static const char_t* NUM_ACCEPT_STRING = "0123456789";

	static const size_t FILE_WRITE_MAX_BUFFER_SIZE = 65536;

	// Public functions
	size_t FileInput::Read(size_t valCount, int8_t* vals) {
		// Keep reading buffers until all requested values have been found
		size_t readCount = 0;
		int8_t lastVal = 0; bool8_t lastValSet = false;

		while(readCount != valCount) {
			// Read a large buffer from the stream
			char_t buffer[FILE_READ_BUFFER_SIZE + 1];
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0) {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				} else {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0) {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				} else {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0) {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				} else {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

			// Exit the loop if the read failed
			if(!bufferSize)
				break;
			
			// Set the last char of the buffer to a null termination character
			buffer[bufferSize] = 0;

			// Check if the last number got cut off to the previous buffer
			char_t* ptr = buffer;
			if(lastValSet) {
				if(lastVal < 0) {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal -= *ptr - '0';

						// Increment the pointer
						++ptr;
					}
				} else {
					while(*ptr >= '0' && *ptr <= '9') {
						// Add the current digit to the last value
						lastVal *= 10;
						lastVal += *ptr - '0';

						// Increment the pointer
						++ptr;
					}
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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

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
			size_t bufferSize = ReadBuffer(FILE_READ_BUFFER_SIZE, buffer);

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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
	size_t FileOutput::Write(size_t strCount, const char_t** strings, const char_t* sep) {
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
			size_t fileWriteCount = WriteBuffer(output.length(), output.data());

			// Increase the total output count
			writeCount += fileWriteCount;

			// Check if the write was successful
			if(fileWriteCount != output.length())
				return writeCount;
			
			// Clear the output string
			output.clear();
		}

		// Write the output's contents to the file
		size_t fileWriteCount = WriteBuffer(output.length(), output.data());

		// Check if the write was successful
		if(fileWriteCount != output.length())
			return writeCount;

		// Increase the total output count
		writeCount += fileWriteCount;

		return writeCount;
	}
}