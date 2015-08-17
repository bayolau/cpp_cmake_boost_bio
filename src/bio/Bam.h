/*
Copyright (c) 2015, Bayo Lau
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of cpp_cmake_boost_bio nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//
// Created by Bayo Lau on 8/16/15.
//

#ifndef BIO_BAMRECORD_H
#define BIO_BAMRECORD_H

#include <ostream>
#include <seqan/bam_io.h>
#include "References.h"

namespace bayolau {
namespace bio {


template<class Seq_>
class BamRecord {
private:
  seqan::BamFileIn const& bam_;
  seqan::BamAlignmentRecord record_;
  using RefPtr = typename bio::References<decltype(seqan::BamAlignmentRecord::qName), Seq_>::SeqPtr;
  RefPtr ref_;
public:
  template<class String_>
  BamRecord(seqan::BamFileIn& bam, bio::References<String_, Seq_> const& refs) : bam_(bam), record_(), ref_(nullptr) {
    if (atEnd(bam)) return;
    readRecord(record_, bam);
    ref_ = refs[getContigName(record_, bam)];
  };

  seqan::BamAlignmentRecord const& record() const {
    return record_;
  }

  seqan::BamFileIn const& bam() const {
    return bam_;
  }

  // this is a shared pointer!
  RefPtr ref() const {
    return ref_;
  }
};

template<class Seq_>
class BamReader {
  seqan::BamFileIn in_;
  seqan::BamHeader header_;
  bio::References<decltype(seqan::BamAlignmentRecord::qName), Seq_> refs_;
public:
  BamReader(const std::string& filename, const std::string& fai)
          : in_(filename.c_str()), header_(), refs_(fai) {
    readHeader(header_, in_);
  }

  BamReader() = delete;

  BamReader(const BamReader&) = delete;

  BamReader& operator=(const BamReader&) = delete;

  std::unique_ptr<BamRecord<Seq_> > Next() {
    if (atEnd(in_)) return nullptr;
    return std::unique_ptr<BamRecord<Seq_> >(new BamRecord<Seq_>(in_, refs_));
  }
};

template<class Seq_>
void Print(std::ostream& os, BamRecord<Seq_> const& record, bool print_alignment) {
  auto start = record.record().beginPos;
  auto length = getAlignmentLengthInRef(record.record());
  os << record.record().qName
     << " " << getContigName(record.record(), record.bam())
     << "(" << getContigLength(record.record(), record.bam()) << ")"
     << " " << start
     << " " << start + length
     << "\n";
  if (print_alignment and record.ref()) {
    seqan::Align<Seq_> align;
    //pretty hard to enforce constness with seqan's class
    bamRecordToAlignment(align, const_cast<Seq_&>(*record.ref()), record.record());
    os << align << '\n';
  }
}


}
}
#endif //BIO_BAMRECORD_H
