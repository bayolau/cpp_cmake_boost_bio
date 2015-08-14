//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>
#include <seqan/vcf_io.h>

#include "main.h"
#include "Options.h"

namespace bayolau {
namespace vcftest {
int main(const bayolau::CommandLine& cl) {

  Options po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }

  std::cout << "vcftest operating with " << po.input() << std::endl;

  seqan::VcfFileIn vcfFileIn(po.input().c_str());
  seqan::VcfFileOut vcfFileOut(vcfFileIn, std::cout, seqan::Vcf());

  seqan::VcfHeader header;
  readHeader(header, vcfFileIn);
  writeHeader(vcfFileOut, header);

  seqan::VcfRecord record;
  while (!atEnd(vcfFileIn)) {
    readRecord(record, vcfFileIn);
    writeRecord(vcfFileOut, record);
  }

  return 0;
}
}
}
