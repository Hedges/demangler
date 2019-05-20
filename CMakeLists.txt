cmake_minimum_required(VERSION 2.6)

#configuration
project("demangler")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/)

option(DEMANGLER_BUILD_EXECUTABLE "Build a command-line executable to demangle" ON)
option(DEMANGLER_BUILD_TESTS "Build the demangler tests" ON)
set(WARNING_FLAGS
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -pedantic
  -Wold-style-cast
  -Wcast-align
  -Wunused
  -Woverloaded-virtual
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -Wformat=2
)

#includes directories
include_directories(
  include/
)

#sources files
set(SRCS
  src/demangler.cpp
  src/details/CVQualifiers.cpp
  src/details/node/ArrayType.cpp
  src/details/node/BareFunctionType.cpp
  src/details/node/BuiltinSubstitution.cpp
  src/details/node/BuiltinType.cpp
  src/details/node/Constructor.cpp
  src/details/node/Decltype.cpp
  src/details/Node.cpp
  src/details/node/Encoding.cpp
  src/details/node/Expression.cpp
  src/details/node/ExprPrimary.cpp
  src/details/node/Holder.cpp
  src/details/node/LocalName.cpp
  src/details/node/MangledName.cpp
  src/details/node/Name.cpp
  src/details/node/NestedName.cpp
  src/details/node/Number.cpp
  src/details/node/OperatorName.cpp
  src/details/node/Prefix.cpp
  src/details/node/SeqId.cpp
  src/details/node/SourceName.cpp
  src/details/node/Substitution.cpp
  src/details/node/TemplateArg.cpp
  src/details/node/TemplateArgs.cpp
  src/details/node/TemplateParam.cpp
  src/details/node/Type.cpp
  src/details/node/UnqualifiedName.cpp
  src/details/node/UnresolvedName.cpp
  src/details/node/UnscopedName.cpp
  src/details/node/UnscopedTemplateName.cpp
  src/details/node/UserSubstitution.cpp
  src/details/Parser.cpp
  src/details/State.cpp
)

#binaries
add_library(demangler ${SRCS})
target_compile_options(demangler PRIVATE ${WARNING_FLAGS} -ggdb3)
set_property(TARGET demangler PROPERTY CXX_STANDARD 17)
set_property(TARGET demangler PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

if (DEMANGLER_BUILD_EXECUTABLE)
  add_executable(demangler_cli src/main.cpp)
  target_link_libraries(demangler_cli demangler)
endif()

if (DEMANGLER_BUILD_TESTS)
  add_subdirectory(tests)
endif()
