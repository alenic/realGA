cmake_minimum_required(VERSION 3.5)
project(realGA)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")


# Headers folder
include_directories(include)

# Source file
file(GLOB SOURCES "src/*.cpp")

# Shared library
add_library(realga_shared SHARED ${SOURCES})

#Generate the static library from the sources
add_library(realga_static STATIC ${SOURCES})

set_target_properties(realga_static realga_shared
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs"
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
#====================================== Build Tests ====================================
file(GLOB TESTSOURCES "tests/*.cpp")
add_executable(tests.bin ${TESTSOURCES})
target_link_libraries(tests.bin realga_static)

set_target_properties(tests.bin
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

# ====================================== Build Benchmarks ====================================
file(GLOB BENCHSOURCES "benchmarks/*.cpp")
add_executable(benchmarks.bin ${BENCHSOURCES} "tests/testcommon.cpp")
target_link_libraries(benchmarks.bin realga_static)
set_target_properties(benchmarks.bin
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/benchmarks")

# ====================================== Build examples ====================================
file(GLOB example_files "examples/*.cpp")
foreach(ex_file ${example_files})
  get_filename_component(EX_FILENAME ${ex_file} NAME_WE)
  add_executable("${EX_FILENAME}.bin" examples/${EX_FILENAME}.cpp)
  target_link_libraries(${EX_FILENAME}.bin realga_static)
  set_target_properties("${EX_FILENAME}.bin" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples")
endforeach()

if(WIN32)
  target_link_libraries(multithread.bin realga_static)
endif(WIN32)

if(UNIX)
  target_link_libraries(multithread.bin realga_static pthread)
endif(UNIX)
