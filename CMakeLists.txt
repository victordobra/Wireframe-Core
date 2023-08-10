# Include the local .build file
include(./.build.cmake OPTIONAL RESULT_VARIABLE LOCAL_BUILD)
message(STATUS "Local .build file included successfully.")

# Build the allocator library
add_subdirectory(allocator)

# Set the minumum required version
cmake_minimum_required(VERSION 3.0.0)
message(STATUS "CMake minimum required version set to ${CMAKE_MIN_VERSION}.")

# Create the project
project(Wireframe-Core VERSION 1.0.0)
message(STATUS "Project name set to \"${PROJECT_NAME}\".")
message(STATUS "Project version set to ${PROJECT_VERSION}.")

# Enable testing
include(CTest)
enable_testing()
message(STATUS "Testing enabled successfully")

# Find the source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(LENGTH SOURCES SOURCE_COUNT)

if(SOURCE_COUNT EQUAL 0)
    message(FATAL_ERROR "ERROR: No sources found!")
elseif(SOURCE_COUNT EQUAL 1)
    message(STATUS "1 source found.")
else()
    message(STATUS "${SOURCE_COUNT} sources found.")
endif()

# Create the library
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
message(STATUS "Library created successfully.")

# Add the include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/allocator/include)
message(STATUS "Include directories added.")

# Add the link directory and target the link library
target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/allocator)
message(STATUS "Core link directory added")
target_link_libraries(${PROJECT_NAME} PUBLIC Wireframe-Allocator)
message(STATUS "Core link library added")

# Find the test source files
file(GLOB_RECURSE TESTS ${PROJECT_SOURCE_DIR}/test/*.cpp)
list(LENGTH TESTS TEST_COUNT)

if(TEST_COUNT EQUAL 0)
    message(FATAL_ERROR "ERROR: No test sources found!")
elseif(TEST_COUNT EQUAL 1)
    message(STATUS "1 test source found.")
else()
    message(STATUS "${TEST_COUNT} test sources found.")
endif()

# Create the test executable
add_executable(Wireframe-Core-Test ${TESTS})
target_compile_features(Wireframe-Core-Test PUBLIC cxx_std_20)
message(STATUS "Test executable created successfully.")

# Add the test include directories
target_include_directories(Wireframe-Core-Test PRIVATE ${PROJECT_SOURCE_DIR}/test PUBLIC ${PROJECT_SOURCE_DIR}/include)
message(STATUS "Test include directories added.")

# Add the link directory and link the core library
target_link_directories(Wireframe-Core-Test PUBLIC ${PROJECT_BINARY_DIR})
message(STATUS "Test link directory added.")
target_link_libraries(Wireframe-Core-Test ${PROJECT_NAME})
message(STATUS "Test link library added.")

# Add CMake components
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
message(STATUS "CPack components added.")