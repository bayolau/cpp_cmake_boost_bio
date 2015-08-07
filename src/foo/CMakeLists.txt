add_library(foo foo.cpp)
install (TARGETS foo DESTINATION bin)
install (FILES foo.h DESTINATION include)
