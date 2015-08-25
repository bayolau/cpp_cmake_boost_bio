include(ExternalProject)
# this builds and installs external libraries into ${OPT_DIR}/include, lib
# the point here is a dummy-proof way to build,
# yet to avoid multiple time-consuming compilations of boost as much as possible
if (BUILD_HDF5 AND NOT EXISTS ${OPT_DIR}/include/H5Cpp.h )
  ExternalProject_Add( hdf5
                       PREFIX ${OPT_DIR}/hdf5_cmake_prefix
                       URL https://www.hdfgroup.org/ftp/HDF5/releases/hdf5-1.8.9/src/hdf5-1.8.9.tar.gz
                       CONFIGURE_COMMAND ./configure --prefix ${OPT_DIR} --enable-cxx
                       BUILD_IN_SOURCE 1
                       BUILD_COMMAND make
                       INSTALL_COMMAND make install
                     )
endif (BUILD_HDF5 AND NOT EXISTS ${OPT_DIR}/include/H5Cpp.h )
