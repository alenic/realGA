cmake_minimum_required(VERSION 3.5)
project(realGen)


set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")

# Headers folder
include_directories(include)

# Source file
file(GLOB SOURCES "src/*.cpp")

# Shared library
add_library(realgen_shared SHARED ${SOURCES})

#Generate the static library from the sources
add_library(realgen_static STATIC ${SOURCES})

set_target_properties(realgen_static realgen_shared
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Libs"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Libs"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Libs"
)

# ====================================== Build TESTS ====================================
file(GLOB TESTSOURCES "test/*.cpp")
add_executable(test_main ${TESTSOURCES})
target_link_libraries(test_main realgen_static)

set_target_properties(test_main
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Tests")

# ====================================== Build examples ====================================
# --------------- readme.cpp -------------------------
add_executable(ex_readme examples/readme.cpp)
target_link_libraries(ex_readme realgen_static)

# --------------- quadratic.cpp -------------------------
add_executable(ex_quadratic examples/quadratic.cpp)
target_link_libraries(ex_quadratic realgen_static)

# --------------- multithread.cpp -------------------------
add_executable(ex_multithread examples/multithread.cpp)

if(WIN32)
  target_link_libraries(ex_multithread realgen_static)
endif(WIN32)

if(UNIX)
  target_link_libraries(ex_multithread realgen_static pthread)
endif(UNIX)

# --------------- Output directories -------------------------
set_target_properties(ex_readme ex_quadratic ex_multithread
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Examples")
