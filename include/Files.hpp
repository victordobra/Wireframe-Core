#pragma once

#include "Defines.hpp"
#include "String.hpp"

namespace wfe {
	/// @brief Represents a file input stream.
	class FileInput {
	public:
		/// @brief The possible flags for the file input stream's type.
		typedef enum {
			/// @brief A normal file input stream type.
			STREAM_TYPE_NORMAL = 0,
			/// @brief Reads the binary contents of the file.
			STREAM_TYPE_BINARY = 1,
			/// @brief Sets the stream's position to the end of the file.
			STREAM_TYPE_AT_THE_END = 2
		} StreamTypeFlags;
		/// @brief Holds all flags for a file input stream.
		typedef uint32_t StreamType;

		/// @brief The possible frames of reference for setting the file input stream's position.
		typedef enum {
			/// @brief Set the stream's position relative to the beginning of the file.
			SET_POS_RELATIVE_BEGIN,
			/// @brief Set the stream's position relative to the end of the file.
			SET_POS_RELATIVE_END,
			/// @brief Set the stream's position relative to its previous position.
			SET_POS_RELATIVE_POS
		} SetPosRelative;

		/// @brief Creates a closed file input stream.
		FileInput() = default;
		FileInput(const FileInput&) = delete;
		FileInput(FileInput&&) noexcept = delete;
		/// @brief Creates a file input stream that reads from the file at the given path.
		/// @param filePath The path to the desired file.
		/// @param streamType The file input stream's type.
		FileInput(const string& filePath, StreamType streamType = STREAM_TYPE_NORMAL);

		FileInput& operator=(const FileInput&) = delete;
		FileInput& operator=(FileInput&&) = delete;

		/// @brief Opens the file input stream for reading from the file at the given path.
		/// @param filePath The path to the desired file.
		/// @param streamType The file input stream's type.
		/// @return True if the file input stream opened successfully, otherwise false.
		bool8_t Open(const string& filePath, StreamType streamType = STREAM_TYPE_NORMAL);
		/// @brief Closes the file input stream.
		/// @return True if the file input stream closed successfully, otherwise false.
		bool8_t Close();

		/// @brief Gets the next character in the file input stream.
		/// @brief Opens the file output stream for writing to the file at the given path.
		/// @return The next character in the file input stream, or the null termination character if the end of the file was reached.
		char_t Get();
		/// @brief Reads a buffer from the given file input stream.
		/// @param size The size of the buffer to read.
		/// @param buffer A pointer to the buffer to be read.
		/// @return The number of bytes written into the buffer.
		size_t ReadBuffer(size_t size, void* buffer);

		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, int8_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, int16_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, int32_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, int64_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, uint8_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, uint16_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, uint32_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, uint64_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, float32_t* vals);
		/// @brief Reads multiple values from the input stream.
		/// @param valCount The maximum number of values to read.
		/// @param vals A pointer to the array in which the values will be written.
		/// @return The number of values read from the input stream.
		size_t Read(size_t valCount, float64_t* vals);
		/// @brief Reads multiple string separated by the given characters.
		/// @param strCount The maximum number of strings to read.
		/// @param strings A pointer to the array in which the string will be written.
		/// @param sep A string containing all the characters that could separate the read strings.
		/// @return The number of read strings.
		size_t Read(size_t strCount, string* strings, const char_t* sep = " \r\n");
	
		/// @brief Gets the file input stream's current position relative to the beginning of the file.
		/// @return The file input stream's current position.
		uint64_t GetPos() const;
		/// @brief Sets the file input stream's position.
		/// @param pos The new position of the input stream.
		/// @param setRel The frame of reference of the given position.
		/// @return True if the stream position was set successfully, otherwise false.
		bool8_t SetPos(int64_t pos, SetPosRelative setRel);

		/// @brief Checks if the file input stream is open.
		/// @return True if the file input stream is open, otherwise false.
		bool8_t IsOpen() const;
		/// @brief Checks if the current stream position is at the end of the file.
		/// @return True if the current stream position is at the end of the file, otherwise false.
		bool8_t IsAtTheEnd() const;
		/// @brief Reads the file's size.
		/// @return The file's size.
		uint64_t GetSize() const;

		/// @brief Closes the file input stream.
		~FileInput();
	private:
		void* internalData = nullptr;
	};

	/// @brief Represents a file output stream.
	class FileOutput {
	public:
		/// @brief The possible flags for the file output stream's type.
		typedef enum {
			/// @brief A normal file output stream type.
			STREAM_TYPE_NORMAL = 0,
			/// @brief Writes binary contents to the file.
			STREAM_TYPE_BINARY = 1,
			/// @brief Appends all new data to the previous data from the file instead of erasing it.
			STREAM_TYPE_APPEND = 2
		} StreamTypeFlags;
		/// @brief Holds all flags for a file input stream.
		typedef uint32_t StreamType;

		/// @brief The possible frames of reference for setting the file output stream's position.
		typedef enum {
			/// @brief Set the stream's position relative to the beginning of the file.
			SET_POS_RELATIVE_BEGIN,
			/// @brief Set the stream's position relative to the end of the file.
			SET_POS_RELATIVE_END,
			/// @brief Set the stream's position relative to its previous position.
			SET_POS_RELATIVE_POS
		} SetPosRelative;

		/// @brief Creates a closed file output stream.
		FileOutput() = default;
		FileOutput(const FileOutput&) = delete;
		FileOutput(FileOutput&&) noexcept = delete;
		/// @brief Crelates a file output stream that writes to the file at the given path.
		/// @param filePath The path to the desired file.
		/// @param streamType The file output stream's type.
		FileOutput(const string& filePath, StreamType streamType = STREAM_TYPE_NORMAL);

		FileOutput& operator=(const FileOutput&) = delete;
		FileOutput& operator=(FileOutput&&) noexcept = delete;

		/// @brief Opens the file output stream for writing to the file at the given path.
		/// @param filePath The path to the desired file.
		/// @param streamType The file output stream's type.
		/// @return True if the file output stream opened successfully, otherwise false.
		bool8_t Open(const string& filePath, StreamType streamType = STREAM_TYPE_NORMAL);
		/// @brief Closes the file input stream.
		/// @return True if the file output stream closed successfully, otherwise false.
		bool8_t Close();

		/// @brief Writes the given buffer to the file output stream.
		/// @param size The suze of the buffer to write.
		/// @param buffer A pointer to the buffer to write.
		/// @return The number of bytes successfully written to the file output stream.
		size_t WriteBuffer(size_t size, const void* buffer);

		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t valCount, const int8_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t valCount, const int16_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of values successfully written to the output stream.
		size_t Write(size_t valCount, const int32_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t valCount, const int64_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of characters values successfully written to the output stream.
		size_t Write(size_t valCount, const uint8_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of characters values successfully written to the output stream.
		size_t Write(size_t valCount, const uint16_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t valCount, const uint32_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param sep The string separating the written values.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t valCount, const uint64_t* vals, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param precision The number of digits to be written from the floating point number.
		/// @param sep The string separating the written values.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t valCount, const float32_t* vals, size_t precision = 10, const char_t* sep = " ");
		/// @brief Writes the given values to the output stream.
		/// @param valCount The number of values to write.
		/// @param vals A pointer to the array of values to write.
		/// @param precision The number of digits to be written from the floating point number.
		/// @param sep The string separating the written values.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t valCount, const float64_t* vals, size_t precision = 10, const char_t* sep = " ");
		/// @brief Writes the given string to the output stream.
		/// @param strCount The number of strings to write.
		/// @param strings The strings to write.
		/// @param sep THe string separating the written strings.
		/// @return The number of characters successfully written to the output stream.
		size_t Write(size_t strCount, const string* strings, const char_t* sep = " ");

		/// @brief Flushes all of the file's changes to disk.
		/// @return True if the changes were flushed successfully, otherwise false.
		bool8_t Flush();

		/// @brief Gets the file output stream's current position relative to the beginning of the file.
		/// @return The file output stream's current position.
		uint64_t GetPos() const;
		/// @brief Sets the file output stream's position.
		/// @param pos The new position of the output stream.
		/// @param setRel The frame of reference of the given position.
		/// @return True if the stream position was set successfully, otherwise false.
		bool8_t SetPos(int64_t pos, SetPosRelative setRel);

		/// @brief Checks if the file output stream is open.
		/// @return True if the file output stream is open, otherwise false.
		bool8_t IsOpen() const;
		/// @brief Checks if the current stream position is at the end of the file.
		/// @return True if the current stream position is at the end of the file, otherwise false.
		bool8_t IsAtTheEnd() const;
		/// @brief Reads the file's size.
		/// @return The file's size.
		uint64_t GetSize() const;

		/// @brief Closes the file output stream.
		~FileOutput();
	private:
		void* internalData = nullptr;
	};
}