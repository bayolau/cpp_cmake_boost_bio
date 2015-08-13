if (USE_SEQAN)
  message(STATUS "using seqan" )
  include_directories("${OPT_DIR}/seqan/include")
  set( SEQAN_FLAGS "-DSEQAN_ENABLE_DEBUG=0 -DSEQAN_ENABLE_TESTING=0 -DSEQAN_HAS_BZIP=1 -DSEQAN_HAS_ZLIB=1" )
  set( SEQAN_FLAGS "${SEQAN_FLAGS} -W -Wall -Wno-long-long -pedantic -Wno-variadic-macros" )

  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_FLAGS}" )
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lbz2 -lz" )
endif (USE_SEQAN)

add_subdirectory("modules")
set (EXTRA_LIBS ${EXTRA_LIBS} modules)
add_subdirectory("modules/bamtest")
set (EXTRA_LIBS ${EXTRA_LIBS} bamtest)
add_subdirectory("modules/sandbox")
set (EXTRA_LIBS ${EXTRA_LIBS} sandbox)

set (EXTRA_LIBS ${EXTRA_LIBS} libboost_program_options.a)

add_executable(bare bare.cpp)
add_dependencies(bare buildinfo)
target_link_libraries(bare ${EXTRA_LIBS})

install (TARGETS bare DESTINATION bin)
