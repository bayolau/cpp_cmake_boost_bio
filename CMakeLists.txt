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

set (OPT_INCLUDE ${OPT_DIR}/include)
set (OPT_LIB ${OPT_DIR}/lib)
include_directories(${OPT_INCLUDE})
link_directories(${OPT_LIB})

# get git status
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
  COMMAND git diff --quiet
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  RESULT_VARIABLE GIT_UNCOMMITED_CHANGE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (GIT_UNCOMMITED_CHANGE)
  set (GIT_DIRTY_FLAG "-dirty")
endif()

# write CMake settings to a header
configure_file (
  "${SRC_DIR}/BuildInfo.h.in"
  "${PROJECT_BINARY_DIR}/BuildInfo.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# deal with the source directory
add_subdirectory(${SRC_DIR})
add_subdirectory(${OPT_DIR})
