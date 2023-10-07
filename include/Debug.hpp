#pragma once

#include "Defines.hpp"
#include <stdarg.h>

namespace wfe {
	/// @brief Formats the given string.
	/// @param dest The target string for the format.
	/// @param maxSize The max size of the target string.
	/// @param format The format to write to the string.
	void FormatString(char_t* dest, size_t maxSize, const char_t* format, ...);
	/// @brief Formats the given string, using the given args list.
	/// @param dest The target string for the format.
	/// @param maxSize The max size of the target string.
	/// @param format The format to write to the string.
	/// @param args The arguments to use for the format.
	void FormatStringV(char_t* dest, size_t maxSize, const char_t* format, va_list args);

	typedef enum {
		LOG_LEVEL_FATAL,
		LOG_LEVEL_ERROR,
		LOG_LEVEL_WARNING,
		LOG_LEVEL_INFO,
		LOG_LEVEL_DEBUG,
		LOG_LEVEL_TRACE
	} LogLevel;

	/// @brief Creates the debug logger.
	/// @param logFilePath The path to the file in which all logged messages will be written
	void CreateLogger(const char_t* logFilePath);
	/// @brief Destroys the debug logger.
	void DestroyLogger();

	/// @brief Logs a message.
	/// @param level The message level.
	/// @param format The message string format.
	void LogMessage(LogLevel level, const char_t* format, ...);
	/// @brief Logs a message, using the given args list to format it.
	/// @param level The message level.
	/// @param format The message string format.
	/// @param args The message string format args.
	void LogMessageV(LogLevel level, const char_t* format, va_list args);

	/// @brief Reports an assertion failure.
	/// @param expression The expression that failed the assertion.
	/// @param message The message to use when logging the assertion failure.
	/// @param file The file where the failure occured.
	/// @param line The line where the failure occured.
	void ReportAssertionFailure(const char_t* expression, const char_t* message = nullptr, const char_t* file = nullptr, size_t line = -1);

#ifdef _MSC_VER
#include <intrin.h>
#define WFE_DEBUG_BREAK() __debugbreak()
#else
#define WFE_DEBUG_BREAK() __builtin_trap()
#endif

/// @brief Defined when warnings are enabled for the logger.
#define WFE_LOG_WARNING_ENABLED
/// @brief Defined when info messages are enabled for the logger.
#define WFE_LOG_INFO_ENABLED

#if defined(WFE_BUILD_MODE_DEBUG)
/// @brief Defined when debug messages are enabled for the logger.
#define WFE_LOG_DEBUG_ENABLED
/// @brief Defined when trace messages are enabled for the logger.
#define WFE_LOG_TRACE_ENABLED
#endif

/// @brief Logs a fatal error.
/// @param format The message string format.
#define WFE_LOG_FATAL(format, ...) wfe::LogMessage(wfe::LOG_LEVEL_FATAL, format, ##__VA_ARGS__);
/// @brief Logs an error.
/// @param format The message string format.
#define WFE_LOG_ERROR(format, ...) wfe::LogMessage(wfe::LOG_LEVEL_ERROR, format, ##__VA_ARGS__);

#ifdef WFE_LOG_WARNING_ENABLED
/// @brief Logs a warning.
/// @param format The message string format.
#define WFE_LOG_WARNING(format, ...) wfe::LogMessage(wfe::LOG_LEVEL_WARNING, format, ##__VA_ARGS__);
#else
/// @brief Logs a warning.
/// @param format The message string format.
#define WFE_LOG_WARNING(format, ...)
#endif

#ifdef WFE_LOG_INFO_ENABLED
/// @brief Logs an info message.
/// @param format The message string format.
#define WFE_LOG_INFO(format, ...) wfe::LogMessage(wfe::LOG_LEVEL_INFO, format, ##__VA_ARGS__);
#else
/// @brief Logs an info message.
/// @param format The message string format.
#define WFE_LOG_INFO(format, ...)
#endif

#ifdef WFE_LOG_DEBUG_ENABLED
/// @brief Logs a debug message.
/// @param format The message string format.
#define WFE_LOG_DEBUG(format, ...) wfe::LogMessage(wfe::LOG_LEVEL_DEBUG, format, ##__VA_ARGS__);
#else
/// @brief Logs a debug message.
/// @param format The message string format.
#define WFE_LOG_DEBUG(format, ...)
#endif

#ifdef WFE_LOG_TRACE_ENABLED
/// @brief Logs a trace message.
/// @param format The message string format.
#define WFE_LOG_TRACE(format, ...) wfe::LogMessage(wfe::LOG_LEVEL_TRACE, format, ##__VA_ARGS__);
#else
/// @brief Logs a trace message.
/// @param format The message string format.
#define WFE_LOG_TRACE(format, ...)
#endif

/// @brief Asserts an expresion and throws an error if that assertion is not true.
#define WFE_ASSERT(expression, message) if(expression) {} else { wfe::ReportAssertionFailure(#expression, message, __FILE__, __LINE__); }
}