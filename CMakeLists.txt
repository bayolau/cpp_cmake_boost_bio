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

# regenerate config file at build time
configure_file ( "${SRC_DIR}/util/BuildInfo.h.cmake.in" "${PROJECT_BINARY_DIR}/BuildInfo.h.cmake" )
add_custom_target(buildinfo ALL
                  DEPENDS
                    ${PROJECT_BINARY_DIR}/BuildInfo.h.cmake
                    ${SRC_DIR}/util/BuildInfo.h.in
                  COMMAND ${CMAKE_COMMAND} -P ${PROJECT_BINARY_DIR}/BuildInfo.h.cmake)
include_directories("${PROJECT_BINARY_DIR}")

# deal with the source directory
add_subdirectory(${SRC_DIR})
add_subdirectory(${OPT_DIR})
