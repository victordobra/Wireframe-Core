#include "BuildInfo.hpp"

#ifdef WFE_PLATFORM_WINDOWS

#include "Files.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace wfe {
	// Internal helper functions
	static void FormatDirPath(string& dirPath, string& formattedPath) {
		// Add a slash at the end of the directory's path, if not already there
		if(dirPath.back() != '/')
			dirPath.push_back('/');

		// Replace every forward slash in the given path with backslashes
		formattedPath = dirPath;
		for(size_t charPos = formattedPath.find('/', 0); charPos != SIZE_T_MAX && charPos != formattedPath.size() - 1; charPos = formattedPath.find('/', charPos + 1))
			formattedPath[charPos] = '\\';
	}
	static void ScanDirChildren(const string& dirPath, const string& formattedPath, vector<string>* files, vector<string>* dirs) {
		// Open the directory to scan all of its files
		WIN32_FIND_DATAA fileInfo;
		HANDLE fileScan = FindFirstFileExA(formattedPath.c_str(), FindExInfoBasic, &fileInfo, FindExSearchNameMatch, nullptr, 0);

		if(fileScan == INVALID_HANDLE_VALUE)
			throw Exception("Failed to scan directory for files!");

		// Loop through all available files and store them in each corresponding vector
		do {
			// Skip the current file if it represents the current or previous directory
			if(!strncmp(fileInfo.cFileName, ".", MAX_PATH) || !strncmp(fileInfo.cFileName, "..", MAX_PATH))
				continue;

			// Check if the current file is a directory
			bool isDir = fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			if(isDir && dirs) {
				dirs->push_back(dirPath + fileInfo.cFileName + '/');
			} else if(!isDir && files) {
				files->push_back(dirPath + fileInfo.cFileName);
			}
		} while(FindNextFileA(fileScan, &fileInfo));

		// Check for any unexpected errors
		if(GetLastError() != ERROR_NO_MORE_FILES)
			throw Exception("Failed to scan directory for files!");
		
		// Close the directory scan
		FindClose(fileScan);
	}
	static void ScanDirReccursive(const string& dirPath, const string& formattedPath, vector<string>& files) {
		// Get the current directory's children files and directories
		vector<string> dirs;
		ScanDirChildren(dirPath, formattedPath, &files, &dirs);

		// Loop through the children directories
		for(const string& nextDirPath : dirs) {
			// Only add the new part to the old formatted path and replace the only slash
			string nextFormattedPath = formattedPath;

			nextFormattedPath.pop_back();
			nextFormattedPath.append(nextDirPath.c_str() + dirPath.size());
			nextFormattedPath.back() = '\\';
			nextFormattedPath.push_back('*');

			// Scan the child directory for files
			ScanDirReccursive(nextDirPath, nextFormattedPath, files);
		}
	}

	// Public functions
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

	vector<string> GetDirectoryChildrenFiles(const string& dirPath) {
		// Format the given directory path
		string fullDirPath = dirPath, formattedPath;
		FormatDirPath(fullDirPath, formattedPath);

		// Append a wildcard character at the end for the search
		formattedPath.push_back('*');

		// Scan the directory for files
		vector<string> files;
		ScanDirChildren(fullDirPath, formattedPath, &files, nullptr);

		return files;
	}
	vector<string> GetDirectoryChildrenDirectories(const string& dirPath) {
		// Format the given directory path
		string fullDirPath = dirPath, formattedPath;
		FormatDirPath(fullDirPath, formattedPath);

		// Append a wildcard character at the end for the search
		formattedPath.push_back('*');

		// Scan the directory for children dirs
		vector<string> dirs;
		ScanDirChildren(fullDirPath, formattedPath, nullptr, &dirs);

		return dirs;
	}
	vector<string> GetDirectoryFiles(const string& dirPath) {
		// Format the given directory path
		string fullDirPath = dirPath, formattedPath;
		FormatDirPath(fullDirPath, formattedPath);

		// Append a wildcard character at the end for the search
		formattedPath.push_back('*');

		// Scan the directory reccursively for files
		vector<string> files;
		ScanDirReccursive(fullDirPath, formattedPath, files);

		return files;
	}
}

#endif