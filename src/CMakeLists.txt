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

add_library( allpathslg
    ${OPT_DIR}/allpathslg/src/TokenizeString.cc
    ${OPT_DIR}/allpathslg/src/feudal/Mempool.cc
    ${OPT_DIR}/allpathslg/src/feudal/Generic.cc
    ${OPT_DIR}/allpathslg/src/feudal/Oob.cc
    ${OPT_DIR}/allpathslg/src/feudal/CharString.cc
    ${OPT_DIR}/allpathslg/src/feudal/BinaryStream.cc
    ${OPT_DIR}/allpathslg/src/system/file/Directory.cc
    ${OPT_DIR}/allpathslg/src/system/file/FileReader.cc
    ${OPT_DIR}/allpathslg/src/system/file/SymLink.cc
    ${OPT_DIR}/allpathslg/src/system/file/File.cc
    ${OPT_DIR}/allpathslg/src/system/file/FileWriter.cc
    ${OPT_DIR}/allpathslg/src/system/file/TempFile.cc
    ${OPT_DIR}/allpathslg/src/system/AllOfOutput.cc
    ${OPT_DIR}/allpathslg/src/system/HostName.cc
    ${OPT_DIR}/allpathslg/src/system/ParallelSortTest.cc
    ${OPT_DIR}/allpathslg/src/system/System.cc
    ${OPT_DIR}/allpathslg/src/system/UseGDB.cc
    ${OPT_DIR}/allpathslg/src/system/Assert.cc
    ${OPT_DIR}/allpathslg/src/system/HTMLUtils.cc
    ${OPT_DIR}/allpathslg/src/system/ParsedArgs.cc
    ${OPT_DIR}/allpathslg/src/system/Utils.cc
    ${OPT_DIR}/allpathslg/src/system/Crash.cc
    ${OPT_DIR}/allpathslg/src/system/MemTracker.cc
    ${OPT_DIR}/allpathslg/src/system/ProcBuf.cc
    ${OPT_DIR}/allpathslg/src/system/WorklistMP.cc
    ${OPT_DIR}/allpathslg/src/system/ErrNo.cc
    ${OPT_DIR}/allpathslg/src/system/MiscUtil.cc
    ${OPT_DIR}/allpathslg/src/system/RunTime.cc
    ${OPT_DIR}/allpathslg/src/system/TraceVal.cc
    ${OPT_DIR}/allpathslg/src/system/WorklistUtils.cc
    ${OPT_DIR}/allpathslg/src/system/Exit.cc
    ${OPT_DIR}/allpathslg/src/system/Parallel.cc
    ${OPT_DIR}/allpathslg/src/system/SysConf.cc
    ${OPT_DIR}/allpathslg/src/system/Types.cc
    ${OPT_DIR}/allpathslg/src/system/Thread.cc
    ${OPT_DIR}/allpathslg/src/system/ThreadsafeIO.cc
    )
set (EXTRA_LIBS ${EXTRA_LIBS} allpathslg)

add_executable(ccbb ccbb.cpp)
add_dependencies(ccbb buildinfo)
if(TARGET boost)
  add_dependencies(ccbb boost)
endif(TARGET boost)
target_link_libraries(ccbb ${EXTRA_LIBS})

install (TARGETS ccbb DESTINATION bin)
