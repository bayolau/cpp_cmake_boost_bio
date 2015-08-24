cmake_minimum_required (VERSION 3.2.2)

set(CMAKE_BUILD_TYPE Release)

project (Bare)
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

# modules/libraries selection
option (USE_SEQAN "Use seqan library" ON)
option (BUILD_HDF5 "build hdf5 into opt" ON)
option (BUILD_BOOST "build boost into opt" ON)

# language settings
set (CMAKE_CXX_STANDARD 11)

# set up opt structure
set (OPT_DIR ${PROJECT_SOURCE_DIR}/opt)
set (OPT_INCLUDE ${OPT_DIR}/include)
set (OPT_LIB ${OPT_DIR}/lib)
include_directories(${OPT_INCLUDE})
link_directories(${OPT_LIB})

# set up src structure
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

# deal with thrid party stuff
include (${OPT_DIR}/opt.cmake)

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
