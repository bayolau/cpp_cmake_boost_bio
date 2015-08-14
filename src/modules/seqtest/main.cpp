//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>
#include <seqan/seq_io.h>

#include "main.h"
#include "Options.h"

namespace bayolau {
namespace seqtest {

int main(const bayolau::CommandLine& cl) {

  Options po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }

  std::cout << "seqtest operating with " << po.input() << std::endl;


  seqan::SeqFileIn seqFileIn(po.input().c_str());

  switch(seqFileIn.format.tagId) {
    case seqan::Find<seqan::AutoSeqFormat, seqan::Fasta>::VALUE:
      std::cout << "fasta" << std::endl;
      break;
    case seqan::Find<seqan::AutoSeqFormat, seqan::Fastq>::VALUE:
      std::cout << "fastq" << std::endl;
      break;
  }

  seqan::CharString id;
  seqan::Dna5String seq;
  seqan::CharString qual;
  while (!atEnd(seqFileIn)) {
    readRecord(id, seq, qual, seqFileIn);
    std::cout << id << " " << length(seq) << " " << length(qual) << '\n';
  }

  return 0;
}
}
}
