//
// Created by Bayo Lau on 8/11/15.
//
#include "BuildInfo.h"

#include <iostream>
#include <array>
#include <numeric>
#include <boost/lambda/lambda.hpp>

#ifdef USE_SEQAN

#include <seqan/basic.h>
#include <seqan/sequence.h>
#include <seqan/stream.h>

#endif

#ifdef USE_FOO

#include "foo.h"

#endif

#include "util/ProgramOptions.h"
#include "main.h"

#include "Basevector.h"

namespace {
struct BareOptions : public bayolau::ProgramOptions {
  BareOptions(const bayolau::CommandLine& cl) {
    this->AddPositional("module", 1);
    this->Add()("module,m", bayolau::bpo::value<std::string>(&module_)->required(), "module");
    this->Add()("integer,i", bayolau::bpo::value<int>(&integer_)->required(), "integer");
    this->Parse(cl);
  }

  int integer() const {
    return integer_;
  }

private:
  int integer_;
  std::string module_;
};
}
namespace bayolau {
namespace sandbox {
int main(const bayolau::CommandLine& cl) {
  BareOptions po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }

#ifdef USE_FOO
  junk::foo::printTo(std::cout);
#endif
#ifdef USE_SEQAN
  std::cout << seqan::CharString("Using SeqAn!") << std::endl;
#endif

  std::array<int, 10> numbers;
  std::iota(numbers.begin(), numbers.end(), po.integer());
  std::cout << "boost's simple program ";
  std::for_each(numbers.cbegin(), numbers.cend(), std::cout << (boost::lambda::_1 * 3) << " ");
  std::cout << std::endl;

  BaseVecVec reads;

  return 0;
}
}
}

#include "feudal/OuterVec.h"
template class OuterVec<BaseVec>;
#include "feudal/FieldVec.h"
#include "feudal/FieldVecDefs.h"
template class FieldVec<2, MempoolAllocator<unsigned char> >;
#include "feudal/SmallVec.h"
#include "feudal/SmallVecDefs.h"
template class SmallVec<char, MempoolAllocator<char> >;
#include "feudal/FeudalString.h"
#include "feudal/FeudalStringDefs.h"
template class FeudalString<char, std::char_traits<char> >;
