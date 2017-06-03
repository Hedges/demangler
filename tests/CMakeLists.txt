cmake_minimum_required(VERSION 2.6)

#configuration
project("demangler")

#includes directories
include_directories(
  ../include
  ./
)

#sources files
set(SRCS
  main.cpp
  test.cpp
)

#binary
add_executable(demangler_tests ${SRCS})
target_link_libraries(demangler_tests demangler)

