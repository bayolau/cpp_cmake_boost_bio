//
// Created by Bayo Lau on 8/11/15.
//
#include <iostream>

#include <seqan/store.h>
#include <seqan/realign.h>

#include "bio/sambam/seqan/Bam.h"
#include "bio/LocusWindow.h"

#include "main.h"
#include "Options.h"

namespace bayolau {
namespace bamtest {

struct PrintVisitor {
  PrintVisitor(std::ostream& os) : os_(os) { }

  template<typename Itr>
  void operator()(Itr begin, Itr end) {
    for (Itr itr = begin; itr != end; ++itr) {
      Print(os_, **itr, false);
    }
  }

private:
  std::ostream& os_;
};


int main(const bayolau::CommandLine& cl) {

  Options po(cl);
  if (not po.valid() or po.help()) {
    std::cout << po << std::endl;
    return 1;
  }

  LOG(info) << "bamtest operating with input " << po.input();
  LOG(info) << "bamtest operating with FAI " << po.fasta();

  LOG(info) << "full list";
  using Generator = bio::BamReader<seqan::Dna5String>;
  {
    std::unique_ptr<Generator> reader(
            po.input() == "-" ? new Generator(std::cin, po.fasta()) : new Generator(po.input(), po.fasta()));
    size_t count = 0;
    if (auto record = reader->Next()) {
      do {
        Print(std::cout, *record, false);
      } while (record->load(*reader));
    }
    std::cout << "number of records " << count << std::endl;
  }
  /*
  {
    Generator reader(po.input(), po.fasta());
    PrintVisitor visitor(std::cout);
    for (bio::LocusWindow<Generator> window(reader); window; ++window) {
      LOG(info) << "window";
      window.Apply(visitor);
    }
  }
   */
  /*
  {
    seqan::FragmentStore<> store;
    seqan::loadContigs(store, po.fasta().c_str());

    seqan::BamFileIn bamFileIn(po.input().c_str());
    readRecords(store, bamFileIn);
    reAlignment(store,0,1,20,true);
    seqan::AlignedReadLayout layout;
    layoutAlignment(layout, store);
    printAlignment(std::cout, layout, store, 0, 32637620,32640514,0, 100000);

  }
   */

  return 0;
}
}
}
