#include <iostream>
#include "foo.h"

namespace junk {
  void foo::printTo(std::ostream& os) {
    os << "foo::printTo() ha la!" << std::endl;
  }
}
