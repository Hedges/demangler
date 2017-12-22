cmake_minimum_required(VERSION 2.6)

#configuration
project("demangler")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

option(DEMANGLER_BUILD_EXECUTABLE "Build a command-line executable to demangle" ON)
option(DEMANGLER_BUILD_TESTS "Build the demangler tests" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wshadow -ggdb3")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#includes directories
include_directories(
  include/
)

#sources files
set(SRCS
  src/demangler.cpp
  src/details/node/BareFunctionType.cpp
  src/details/node/BuiltinSubstitution.cpp
  src/details/node/BuiltinType.cpp
  src/details/node/Constructor.cpp
  src/details/Node.cpp
  src/details/node/Encoding.cpp
  src/details/node/MangledName.cpp
  src/details/node/Name.cpp
  src/details/node/NestedName.cpp
  src/details/node/OperatorName.cpp
  src/details/node/Prefix.cpp
  src/details/node/SeqId.cpp
  src/details/node/SourceName.cpp
  src/details/node/Substitution.cpp
  src/details/node/TemplateArg.cpp
  src/details/node/TemplateArgs.cpp
  src/details/node/Type.cpp
  src/details/node/UnqualifiedName.cpp
  src/details/node/UnscopedName.cpp
  src/details/node/UnscopedTemplateName.cpp
  src/details/node/UserSubstitution.cpp
  src/details/Parser.cpp
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
