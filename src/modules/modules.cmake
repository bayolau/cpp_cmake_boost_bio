add_library(modules ${CMAKE_CURRENT_LIST_DIR}/dispatch.cpp)
set (EXTRA_LIBS ${EXTRA_LIBS} modules)

#http://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})

#automatically discover modules/<name>/main.cpp and compile into a library named <name>
foreach (sub ${SUBDIRS})
  message(STATUS "adding module ${sub}")
  add_library(${sub} ${CMAKE_CURRENT_LIST_DIR}/${sub}/main.cpp)
  set (EXTRA_LIBS ${EXTRA_LIBS} ${sub})
endforeach (sub ${SUBDIRS})
