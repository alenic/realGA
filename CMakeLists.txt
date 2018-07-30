cmake_minimum_required(VERSION 3.5)
project(realGen)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

# Headers folder
include_directories(include)

# Source file
#file(GLOB SOURCES src/realgen.cpp src/realgenotype.cpp src/stat.cpp)
file(GLOB SOURCES "src/*.cpp")

# Shared library
add_library(realgen_shared SHARED ${SOURCES})

#Generate the static library from the sources
add_library(realgen_static STATIC ${SOURCES})

# ====================================== Build test ====================================
file(GLOB TESTSOURCES "test/*.cpp")
add_executable(test_main ${TESTSOURCES})
# Link with realgen_static library
target_link_libraries(test_main realgen_static)


# ====================================== Build examples ====================================
if (UNIX)
	file(GLOB EX_MULTITHREAD "examples/multithread.cpp")
	add_executable(ex_multithread ${EX_MULTITHREAD})
	target_link_libraries(ex_multithread realgen_static pthread)
endif (UNIX)


if (WIN32)
	file(GLOB EX_MULTITHREAD "examples/multithread.cpp")
	add_executable(ex_multithread ${EX_MULTITHREAD})
	target_link_libraries(ex_multithread realgen_static)
endif (WIN32)

