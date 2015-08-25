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

#ifndef BIO_BAM_H
#define BIO_BAM_H

#include <ostream>
#include <seqan/bam_io.h>
#include "bio/seq/SeqBase.h"
#include "bio/References.h"
#include "bio/Locus.h"

namespace bayolau {
namespace bio {

template<class Seq_>
struct SeqanBamRecord;

template<class Seq_>
struct Trait<SeqanBamRecord<Seq_> > {
  using Seq = Seq_;
  using Impl = seqan::BamAlignmentRecord;
  using String = decltype(Impl::qName);
  using Locus = Locus<decltype(Impl::rID), decltype(Impl::beginPos)>;
  using Refs = typename bio::References<String, Seq_>;
  using RefPtr = typename bio::References<String, Seq_>::SeqPtr;
};

template<class Seq_>
struct SeqanBamRecord : public SeqBase<SeqanBamRecord<Seq_> > {
  using Seq = typename Trait<SeqanBamRecord>::Seq;
  using Impl = typename Trait<SeqanBamRecord>::Impl;
  using String = typename Trait<SeqanBamRecord>::String;
  using Locus = typename Trait<SeqanBamRecord>::Locus;
  using Refs = typename Trait<SeqanBamRecord>::Refs;
  using RefPtr = typename Trait<SeqanBamRecord>::RefPtr;

  SeqanBamRecord(Refs& refs)
          : record_(), cname_(), bam_(nullptr), refs_(refs), ref_end_(-1), mapped_(false) { }

  SeqanBamRecord(seqan::BamFileIn& bam, Refs& refs)
          : record_(), cname_(), bam_(nullptr), refs_(refs), ref_end_(-1), mapped_(false) { _load(bam); };

  void clear() {
    seqan::clear(record_);
    seqan::clear(cname_);
    bam_ = nullptr;
    ref_end_ = -1;
    mapped_ = false;
  }

  seqan::BamFileIn const& bam() const { return *bam_; }

private:
  friend struct SeqBase<SeqanBamRecord>;

  template<class Generator>
  bool _load(Generator& gen) { return _load(gen.impl()); }

  bool _load(seqan::BamFileIn& bam) {
    // we don't want to have any chance of double-operating if load fail
    clear();
    if (atEnd(bam)) { return false; }

    bam_ = &bam;
    readRecord(record_, bam);
    ref_end_ = record_.beginPos;
    if (_isMapped()) {
      cname_ = getContigName(record_, bam);
      mapped_ = refs_.has(cname_);
      ref_end_ += getAlignmentLengthInRef(record_);
    }
    return true;
  };

  Locus _locus() const { return Locus(_ref_id(), _ref_begin0(), _ref_end0()); } //rvo

  bool _isMapped() const { return !hasFlagUnmapped(record_); }

  decltype(Impl::rID) _ref_id() const { return record_.rID; }

  decltype(Impl::beginPos) _ref_begin0() const { return record_.beginPos; }

  decltype(Impl::beginPos) _ref_end0() const { return ref_end_; }

  RefPtr _ref() const { return _isMapped() ? refs_[cname_] : nullptr; }

  Impl const& _impl() const { return record_; }

  Impl& _impl() { return record_; }

  Impl record_;
  String cname_;
  const seqan::BamFileIn* bam_;
  Refs& refs_;
  decltype(Impl::beginPos) ref_end_;
  bool mapped_;
};

template<class Seq_>
struct BamReader {
  using Refs = bio::References<decltype(seqan::BamAlignmentRecord::qName), Seq_>;
  using value_type = SeqanBamRecord<Seq_>;
  using pointer = std::unique_ptr<value_type>;

  BamReader(const std::string& filename, const std::string& fai)
          : in_(filename.c_str()), header_(), refs_(fai) {
    readHeader(header_, in_);
  }

  BamReader(std::istream& stream, const std::string& fai)
          : in_(stream), header_(), refs_(fai) {
    readHeader(header_, in_);
  }

  BamReader() = delete;

  BamReader(const BamReader&) = delete;

  BamReader& operator=(const BamReader&) = delete;

  pointer Next() {
    if (atEnd(in_)) return nullptr;
    return pointer(new SeqanBamRecord<Seq_>(in_, refs_));
  }

  seqan::BamFileIn& impl() { return in_; }

private:
  seqan::BamFileIn in_;
  seqan::BamHeader header_;
  Refs refs_;
};

template<class Seq_>
void Print(std::ostream& os, SeqanBamRecord<Seq_> const& record, bool print_alignment) {
  os << record.impl().qName;
  if (record.isMapped()) {
    os << " " << getContigName(record.impl(), record.bam())
    << " " << record.ref_id()
    << "(" << getContigLength(record.impl(), record.bam()) << ")"
    << " " << record.ref_begin0()
    << " " << record.ref_end0();
  }
  os << "\n";
  if (print_alignment and record.isMapped()) {
    seqan::Align<Seq_> align;
    //pretty hard to enforce constness with seqan's class
    bamRecordToAlignment(align, const_cast<Seq_&>(*record.ref()), record.impl());
    os << align << '\n';
  }
}


}
}
#endif //BIO_BAM_H
