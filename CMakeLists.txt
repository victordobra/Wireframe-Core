cmake_minimum_required(VERSION 3.0.0)
project(MG-Engine-Core VERSION 1.0.0)

include(CTest)
enable_testing()

################ MAIN LIBRARY ################
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(MG-Engine-Core STATIC)
target_sources(MG-Engine-Core PRIVATE ${SOURCES})
target_compile_features(MG-Engine-Core PUBLIC cxx_std_20)

target_include_directories(MG-Engine-Core PRIVATE ${PROJECT_SOURCE_DIR}/src PUBLIC ${PROJECT_SOURCE_DIR}/include)

################ TEST PROJECT ################
file(GLOB_RECURSE TESTS ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(Test)
target_sources(Test PRIVATE ${TESTS})
target_compile_features(Test PUBLIC cxx_std_20)

target_include_directories(Test PRIVATE ${PROJECT_SOURCE_DIR}/test PUBLIC ${PROJECT_SOURCE_DIR}/include )
target_link_directories(Test PUBLIC ${PROJECT_SOURCE_DIR}/build)
target_link_libraries(Test MG-Engine-Core)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
