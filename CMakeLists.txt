cmake_minimum_required (VERSION 3.2.2)

project (Bare)
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

# modules/libraries selection
option (USE_FOO "Use foo" ON)
option (USE_SEQAN "Use seqan library" ON)

# language settings
set (CMAKE_CXX_STANDARD 11)

# directory structures
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set (OPT_DIR ${PROJECT_SOURCE_DIR}/opt)

# write CMake settings to a header
configure_file (
  "${SRC_DIR}/CMakeConfig.h.in"
  "${PROJECT_BINARY_DIR}/CMakeConfig.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# deal with the source directory
add_subdirectory(${SRC_DIR})
