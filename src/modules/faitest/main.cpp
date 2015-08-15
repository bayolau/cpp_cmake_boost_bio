//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>
#include <seqan/seq_io.h>

#include "main.h"
#include "Options.h"

namespace bayolau {
namespace faitest {

int main(const bayolau::CommandLine& cl) {

  Options po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }

  std::cout << "faitest operating with " << po.input() << std::endl;

  seqan::FaiIndex faiIndex;
  if(!open(faiIndex, po.input().c_str())) {
    std::cerr << "Could not open " << po.input() << ".fai" << std::endl;
    return 1;
  }

  typedef seqan::Iterator< decltype(faiIndex.seqNameStore) >::Type Itr;
  Itr b = begin(faiIndex.seqNameStore);
  Itr e = end(faiIndex.seqNameStore);
  std::cout << "seqNameStore size " << std::distance(b,e) << "\n";
  for(Itr itr = b; itr != e ; ++itr) {
    std::cout << *itr << "\n";
  }

  seqan::CharString second = value(faiIndex.seqNameStore,1);
  std::cout << "second entry " << second << std::endl;

  unsigned idx = -1;
  if(not getIdByName(idx, faiIndex, second)) {
    std::cerr << "Could not get first entry index" << std::endl;
  }
  std::cout << "second index " << idx << std::endl;

  unsigned seqLength = sequenceLength(faiIndex, idx);
  std::cout << "second length " << seqLength << std::endl;

  seqan::CharString buffer;
  readRegion(buffer, faiIndex, idx, 0, 1000);
  std::cout << buffer << std::endl;

//  readRegion(buffer, faiIndex, idx);
//  std::cout << buffer << std::endl;

  return 0;
}
}
}
