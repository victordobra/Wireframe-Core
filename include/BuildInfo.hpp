#pragma once

#include <stddef.h>

// Windows
#if defined(WIN32) | defined(_WIN32) | defined(__WIN32__)
/// @brief Defined when building for Windows.
#define WFE_PLATFORM_WINDOWS
#endif

// Linux
#if defined(__linux__) || defined(__gnu_linux__)
/// @brief Defined when building for Linux.
#define WFE_PLATFORM_LINUX
#endif

// Android
#if defined(__ANDROID__)
/// @brief Defined when building for Android.
#define WFE_PLATFORM_ANDROID
#endif

// Unix
#if defined(__unix__)
/// @brief Defined when building for Unix.
#define WFE_PLATFORM_UNIX
#endif

// Posix
#if defined(_POSIX_VERSION)
/// @brief Defined when building for Posix.
#define WFE_PLATFORM_POSIX
#endif

// Apple platforms
#if defined(__APPLE__)
/// @brief Defined when building for an Apple platform.
#define WFE_PLATFORM_APPLE

#include <TargetConditionals.h>

// IOS simulator
#if TARGET_IPHONE_SIMULATOR
/// @brief Defined when building for IOS.
#define WFE_PLATFORM_IOS
/// @brief Defined when building for an IOS simulator.
#define WFE_PLATFORM_IOS_SIMULATOR
#endif

// IOS
#if TARGET_OS_IPHONE
/// @brief Defined when building for IOS.
#define WFE_PLATFORM_IOS
#endif

// Other kinds of Mac OS
#if TARGET_OS_MAC && !TARGET_IPHONE_SIMULATOR && !TARGET_OS_IPHONE
/// @brief Defined when building for other kinds of Mac OS
#define WFE_PLATFORM_MAC_OS
#else
#error "Failed to find the target Apple platform!"
#endif

#endif

// Check if the target platform was detected
#if !defined(WFE_PLATFORM_WINDOWS) && !defined(WFE_PLATFORM_LINUX) && !defined(WFE_PLATFORM_ANDROID) && !defined(WFE_PLATFORM_UNIX) && !defined(WFE_PLATFORM_POSIX) && !defined(WFE_PLATFORM_APPLE)
#error "Failed to find the target platform!"
#endif

// Architecture
#if defined(__i386__)
// Defined when building for a 32-bit architecture
#define WFE_ARCHITECTURE_X86_64
#else
// Defined when building for a 64-bit architecture
#define WFE_ARCHITECTURE_X64
#endif

// Build mode
#if !defined(NDEBUG)
// Defined when building in debug mode
#define WFE_BUILD_MODE_DEBUG
#else
// Defined when building in release mode
#define WFE_BUILD_MODE_RELEASE
#endif