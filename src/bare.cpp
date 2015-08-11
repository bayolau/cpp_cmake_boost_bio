#include <memory>
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

#include "util/ProgramOptions.h"

struct BareOptions : public bayolau::ProgramOptions {
  BareOptions(int argc, char* argv[]) {
    this->Add()("integer,i", bayolau::bpo::value<int>()->required(), "integer");
    this->Parse(argc, argv);
  }
};

int main(int argc, char* argv[]) {
  std::unique_ptr<bayolau::ProgramOptions> po(new BareOptions(argc,argv));
  if (not po->valid() or po->has("help")) {
    std::cout << *po << std::endl;
    return 1;
  }

  std::cout << "build version " << bayolau::BuildInfo::version() << std::endl;
  std::cout << "git status " << bayolau::BuildInfo::git_status() << std::endl;
#ifdef USE_FOO
  junk::foo::printTo(std::cout);
#endif
#ifdef USE_SEQAN
  std::cout << seqan::CharString("Using SeqAn!") << std::endl;
#endif

  std::array<int, 10> numbers;
  std::iota(numbers.begin(), numbers.end(), po->get<int>("integer"));
  std::cout << "boost's simple program ";
  std::for_each(numbers.cbegin(), numbers.cend(), std::cout << (boost::lambda::_1 * 3) << " ");
  std::cout << std::endl;
}
