//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>
#include <seqan/bam_io.h>

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

  std::cout << "bamtest operating with " << po.input() << std::endl;

  seqan::BamFileIn bamFileIn(po.input().c_str());
  seqan::BamFileOut bamFileOut(context(bamFileIn), std::cout, seqan::Sam());

  seqan::BamHeader header;
  readHeader(header, bamFileIn);
  writeHeader(bamFileOut, header);

  seqan::BamAlignmentRecord record;
  while (!atEnd(bamFileIn)) {
    readRecord(record, bamFileIn);
    writeRecord(bamFileOut, record);
  }

  return 0;
}
}
}
