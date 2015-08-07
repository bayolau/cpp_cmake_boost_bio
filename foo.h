#ifndef FOO
#define FOO

#include <iostream>

namespace junk {

  struct foo{
    static void printTo(std::ostream& os) {
      os << "foo::printTo()" << std::endl;
    }
  };
}

#endif
