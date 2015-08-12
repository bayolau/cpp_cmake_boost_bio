if (USE_FOO)
  include_directories("${SRC_DIR}/foo")
  add_subdirectory(foo)
  set (EXTRA_LIBS ${EXTRA_LIBS} foo)
endif (USE_FOO)

if (USE_SEQAN)
  message(STATUS "using seqan" )
  include_directories("${OPT_DIR}/seqan/include")
  set( SEQAN_FLAGS "-DSEQAN_ENABLE_DEBUG=0 -DSEQAN_ENABLE_TESTING=0 -DSEQAN_HAS_BZIP=1 -DSEQAN_HAS_ZLIB=1" )
  set( SEQAN_FLAGS "${SEQAN_FLAGS} -W -Wall -Wno-long-long -pedantic -Wno-variadic-macros" )

  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_FLAGS}" )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lbz2 -lz" )
endif (USE_SEQAN)

set (EXTRA_LIBS ${EXTRA_LIBS} libboost_program_options.a)

add_executable(bare bare.cpp util/ProgramOptions.h modules/dispatch.h modules/dispatch.cpp modules/sandbox/main.h modules/sandbox/main.cpp)
add_dependencies(bare buildinfo)
target_link_libraries(bare ${EXTRA_LIBS})

install (TARGETS bare DESTINATION bin)
