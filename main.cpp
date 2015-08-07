
#include <iostream>

#include "foo.h"

int main(int argc, char* argv[]) {
  std::cout << "hello" << std::endl;
  junk::foo::printTo(std::cout);
}
