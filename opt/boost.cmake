# this builds and installs external libraries into ${OPT_DIR}/include, lib
# the point here is a dummy-proof way to build,
# yet to avoid multiple time-consuming compilations of boost as much as possible

# there are many ways of doing this, e.g. one alternative
# is to build Boost as an External Project in the out-of-source build tree
# but I want to play with modularizing build at some point

if (BUILD_BOOST AND NOT EXISTS ${OPT_DIR}/include/boost )
  set (MODULAR_BOOST_DIR ${OPT_DIR}/modular-boost)
  execute_process (
    COMMAND ./bootstrap.sh --prefix=${OPT_DIR}
    COMMAND ./b2 headers
    COMMAND ./b2
    COMMAND ./b2 install
    WORKING_DIRECTORY ${MODULAR_BOOST_DIR}
  )
endif (BUILD_BOOST AND NOT EXISTS ${OPT_DIR}/include/boost )
