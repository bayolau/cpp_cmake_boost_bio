# this builds and installs external libraries into ${OPT_DIR}/include, lib
# the point here is a dummy-proof way to build,
# yet to avoid multiple time-consuming compilations of boost as much as possible

if (BUILD_BOOST AND NOT EXISTS ${OPT_DIR}/include/boost )
  ExternalProject_Add( boost
                       PREFIX ${OPT_DIR}/boost_cmake_prefix
                       URL http://sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz/download
                       CONFIGURE_COMMAND ./bootstrap.sh --prefix=${OPT_DIR}
                       BUILD_IN_SOURCE 1
                       BUILD_COMMAND ./b2 -j2 COMMAND ./b2 headers -j2
                       INSTALL_COMMAND ./b2 install
                     )
endif (BUILD_BOOST AND NOT EXISTS ${OPT_DIR}/include/boost )
