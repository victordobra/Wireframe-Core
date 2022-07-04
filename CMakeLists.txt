cmake_minimum_required(VERSION 3.0.0)
project(Wireframe-Core VERSION 1.0.0)

include(CTest)
enable_testing()

# Main library
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Test executable
file(GLOB_RECURSE TESTS ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(Test)
target_sources(Test PRIVATE ${TESTS})
target_compile_features(Test PUBLIC cxx_std_20)

target_include_directories(Test PRIVATE ${PROJECT_SOURCE_DIR}/test PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_directories(Test PUBLIC ${PROJECT_SOURCE_DIR}/build)
target_link_libraries(Test ${PROJECT_NAME})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
