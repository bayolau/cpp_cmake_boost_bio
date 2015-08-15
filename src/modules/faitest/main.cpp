//
// Created by Bayo Lau on 8/11/15.
//

#include <iostream>

#include "main.h"
#include "Options.h"

#include "bio/Fai.h"


namespace bayolau {
namespace faitest {

int main(const bayolau::CommandLine& cl) {

  Options po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }

  std::cout << "faitest operating with " << po.input() << std::endl;

  bio::Fai lookup(po.input());
  std::cout << "seqNameStore size " << lookup.size() << std::endl;

  for (auto itr = lookup.name_begin(); itr != lookup.name_end(); ++itr) {
    std::cout << *itr << " " << lookup.size(*itr) << "\n";
  }

  const auto& first = *lookup.name_begin();
  std::cout << "first entry " << first << " (" << lookup.size(first) << ")" << std::endl;
  std::cout << *lookup.get<seqan::Dna5String>(first, 100, 200) << std::endl;
  return 0;
}
}
}
