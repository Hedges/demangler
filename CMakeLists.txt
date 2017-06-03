cmake_minimum_required(VERSION 2.6)

#configuration
project("demangler")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

option(DEMANGLER_BUILD_EXECUTABLE "Build a command-line executable to demangle" ON)
option(DEMANGLER_BUILD_TESTS "Build the demangler tests" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wshadow")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#includes directories
include_directories(
  include/
)

#sources files
set(SRCS
  src/demangler.cpp
  src/details/Demangler.cpp
)

#binaries
add_library(demangler ${SRCS})

if (DEMANGLER_BUILD_EXECUTABLE)
  add_executable(demangler_cli src/main.cpp)
  target_link_libraries(demangler_cli demangler)
endif()

if (DEMANGLER_BUILD_TESTS)
  add_subdirectory(tests)
endif()
