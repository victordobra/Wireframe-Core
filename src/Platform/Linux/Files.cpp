#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_LINUX

#include "Files.hpp"
#include "Memory.hpp"

#include <stdio.h>

namespace wfe {
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