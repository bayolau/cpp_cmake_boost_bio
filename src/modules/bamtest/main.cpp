//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>
#include <seqan/bam_io.h>
#include "bio/References.h"

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

  bio::References<seqan::CharString, Seq> references(po.fasta());

  seqan::BamFileIn bamFileIn(po.input().c_str());
  seqan::BamFileOut bamFileOut(context(bamFileIn), std::cout, seqan::Sam());

  seqan::BamHeader header;
  readHeader(header, bamFileIn);
//  writeHeader(bamFileOut, header);

  seqan::BamAlignmentRecord record;
  seqan::Align<Seq> align;
  while (!atEnd(bamFileIn)) {
    readRecord(record, bamFileIn);
    auto ptr = references[getContigName(record, bamFileIn)];
    if(ptr) {
      bamRecordToAlignment(align, const_cast<Seq&>(*ptr), record); //pretty hard to enforce constness with seqan's class
      std::cout << align << '\n';
    }
//    std::cout << record.qName << "|" << getContigName(record, bamFileIn) << "|" << getAlignmentLengthInRef(record) << "|" << getContigLength(record, bamFileIn) << "\n";
//    writeRecord(bamFileOut, record);
  }

  return 0;
}
}
}
