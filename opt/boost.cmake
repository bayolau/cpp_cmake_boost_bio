# this builds and installs external libraries into ${OPT_DIR}/include, lib
# the point here is a dummy-proof way to build,
# yet to avoid multiple time-consuming compilations of boost as much as possible
include(ExternalProject)

cmake_host_system_information(RESULT NCPU QUERY NUMBER_OF_PHYSICAL_CORES)

set( B2_OPTIONS -sBZIP2_INCLUDE=${OPT_DIR}/include -sBZIP2_LIBPATH=${OPT_DIR}/lib -j${NCPU} )

if (BUILD_BOOST AND NOT EXISTS ${OPT_DIR}/include/boost)
  ExternalProject_Add( boost
                       DEPENDS bzip2
                       PREFIX ${OPT_DIR}/boost_cmake_prefix
                       URL http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz/download
                       PATCH_COMMAND patch -p0 < ${OPT_DIR}/boost.patch
                       CONFIGURE_COMMAND ./bootstrap.sh --prefix=${OPT_DIR}
                       BUILD_IN_SOURCE 1
                       BUILD_COMMAND ./b2 ${B2_OPTIONS} COMMAND ./b2 headers ${B2_OPTIONS}
                       INSTALL_COMMAND ./b2 install
                     )
endif (BUILD_BOOST AND NOT EXISTS ${OPT_DIR}/include/boost)
