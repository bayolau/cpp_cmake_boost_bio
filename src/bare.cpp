#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <boost/lambda/lambda.hpp>

#include "BuildInfo.h"

#ifdef USE_SEQAN
#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/stream.h>
#endif

#ifdef USE_FOO
#include "foo.h"
#endif

int main(int argc, char* argv[]) {
  std::cout << "build version " << bayolau::BuildInfo::version() << std::endl;
  std::cout << "git status " << bayolau::BuildInfo::git_status() << std::endl;
#ifdef USE_FOO
  junk::foo::printTo(std::cout);
#endif
#ifdef USE_SEQAN
  std::cout << seqan::CharString("Using SeqAn!") << std::endl;
#endif
  
  std::array<int,10> numbers;
  std::iota(numbers.begin(), numbers.end(), 100);
  std::cout << "boost's simple program ";
  std::for_each(numbers.begin(),numbers.end(), std::cout << (boost::lambda::_1 * 3) << " ");
  std::cout << std::endl;
}
