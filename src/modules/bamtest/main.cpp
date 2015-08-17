//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>
#include "bio/Bam.h"

#include "main.h"
#include "Options.h"

namespace bayolau {
namespace bamtest {
int main(const bayolau::CommandLine& cl) {

  Options po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }
  using Seq = seqan::Dna5String;

  std::cout << "bamtest operating with " << po.input() << std::endl;
  std::cout << "bamtest operating with " << po.fasta() << std::endl;

  bio::BamReader<Seq> reader(po.input(),po.fasta());
  while(auto record = reader.Next()) {
    Print(std::cout, *record, false);
  }

  return 0;
}
}
}
