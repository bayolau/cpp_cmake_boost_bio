
# there are many ways of doing this, e.g. one alternative
# is to build Boost as an External Project in the out-of-source build tree

# the point here is a dummy-proof way to build,
# yet to avoid multiple time-consuming compilations of boost as much as possible

# this builds and installs Boost into ${OPT_DIR}/include, lib
if (NOT EXISTS ${OPT_DIR}/include/boost )
  set (MODULAR_BOOST_DIR ${OPT_DIR}/modular-boost)
  execute_process (
    COMMAND ./bootstrap.sh --prefix=${OPT_DIR}
    WORKING_DIRECTORY ${MODULAR_BOOST_DIR}
  )
  execute_process (
    COMMAND ./b2 headers
    WORKING_DIRECTORY ${MODULAR_BOOST_DIR}
  )
  execute_process (
    COMMAND ./b2
    WORKING_DIRECTORY ${MODULAR_BOOST_DIR}
  )
  execute_process (
    COMMAND ./b2 install
    WORKING_DIRECTORY ${MODULAR_BOOST_DIR}
  )
endif (NOT EXISTS ${OPT_DIR}/include/boost )
