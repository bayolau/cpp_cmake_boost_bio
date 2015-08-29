//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>

#include <seqan/store.h>
#include <seqan/realign.h>

#include "bio/sambam/seqan/Bam.h"
#include "bio/sambam/local/Sam.h"
//#include "bio/LocusWindow.h"
#include "util/Logging.h"

#include "main.h"
#include "Options.h"

#include <seqan/sequence/sequence_shortcuts.h>

namespace bayolau {


namespace samintest {


struct Test {
  template<class Generator>
  static void Run(Generator& reader) {
    size_t count = 0;
    if (auto record = reader.Next()) {
      do {
        Print(std::cerr,*record, false);
        ++count;
      } while (record->load(reader));
    }
    LOG(info) << "number of records " << count;
  }
};

int main(const bayolau::CommandLine& cl) {

  Options po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }

  LOG(info) << "samintest operating with input " << po.input();
  LOG(info) << "samintest operating with FAI " << po.fasta();

  LOG(info) << "full list";


  if (po.input() == "-") {
//    using Generator = bayolau::bio::BamReader<seqan::Dna5String>;
//    Generator reader(std::cin, po.fasta());
    using Generator = bayolau::bio::StdinSAMReader<seqan::Dna5String>;
    Generator reader(po.fasta());
    Test::Run(reader);
  }
  else {
    using Generator = bayolau::bio::BamReader<seqan::Dna5String>;
    Generator reader(po.input(), po.fasta());
    Test::Run(reader);

  }
//  size_t count = 0;
//  for (std::vector<char> buffer; StdinByLine::Read(buffer);) {
//    ++count;
//  }
//  LOG(info) << "number of records " << count;

  return 0;
}
}
}
