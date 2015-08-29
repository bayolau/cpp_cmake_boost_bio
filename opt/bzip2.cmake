# this builds and installs external libraries into ${OPT_DIR}/include, lib
# the point here is a dummy-proof way to build,
# yet to avoid multiple time-consuming compilations of boost as much as possible


#cmake_host_system_information(RESULT NCPU QUERY NUMBER_OF_PHYSICAL_CORES)

ExternalProject_Add( bzip2
                     PREFIX ${OPT_DIR}/bzip2_cmake_prefix
                     URL http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz
                     URL_MD5 00b516f4704d4a7cb50a1d97e6e8e15b
                     CONFIGURE_COMMAND command
                     BUILD_IN_SOURCE 1
                     BUILD_COMMAND make -f Makefile-libbz2_so
                     INSTALL_COMMAND make install PREFIX=${OPT_DIR}
                   )
