if (USE_FOO)
  include_directories("${SRC_DIR}/foo")
  add_subdirectory(foo)
  set (EXTRA_LIBS ${EXTRA_LIBS} foo)
endif (USE_FOO)

add_executable(bare bare.cpp)
target_link_libraries(bare ${EXTRA_LIBS})

install (TARGETS bare DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)
