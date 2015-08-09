cmake_minimum_required (VERSION 3.2.2)

project (Bare)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

option (USE_FOO "Use foo" ON)
option (USE_SEQAN "Use seqan library" ON)

set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set (OPT_DIR ${PROJECT_SOURCE_DIR}/opt)

configure_file (
  "${SRC_DIR}/CMakeConfig.h.in"
  "${PROJECT_BINARY_DIR}/CMakeConfig.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory(${SRC_DIR})
