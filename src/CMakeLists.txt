#seqan
if (USE_SEQAN)
  message(STATUS "using seqan" )
  include_directories("${OPT_DIR}/seqan/include")
  set( SEQAN_FLAGS "-DSEQAN_ENABLE_DEBUG=0 -DSEQAN_ENABLE_TESTING=0 -DSEQAN_HAS_BZIP=1 -DSEQAN_HAS_ZLIB=1" )
  set( SEQAN_FLAGS "${SEQAN_FLAGS} -W -Wall -Wno-long-long -pedantic -Wno-variadic-macros" )

  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_FLAGS}" )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lbz2 -lz" )
endif (USE_SEQAN)

#boost
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

#automatically discover modules/<name> and compile modules/<name>/main.cpp into a library named <name>
include(modules/modules.cmake)

set (EXTRA_LIBS ${EXTRA_LIBS} boost_program_options boost_log )

add_executable(ccbb ccbb.cpp)
add_dependencies(ccbb buildinfo)
if(TARGET boost)
  add_dependencies(ccbb boost)
endif(TARGET boost)
target_link_libraries(ccbb ${EXTRA_LIBS})

install (TARGETS ccbb DESTINATION bin)
