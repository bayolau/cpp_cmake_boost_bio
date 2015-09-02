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
// Created by bayolau on 8/27/15.
//

#include <utility>

#include "util/Range.h"

#include "bio/Forward.h"
#include "bio/References.h"
#include "bio/seq/SeqBase.h"

#ifndef BIO_SAMBAM_LOCAL_SAM_H
#define BIO_SAMBAM_LOCAL_SAM_H
namespace bayolau {
namespace bio {

template<class Itr>
void ProcessCigar(Itr const begin, const Itr end) {
  char* itr = const_cast<char*>(begin);
  if (itr == end or *itr == '#') {
    std::cout << "empty CIGAR" << std::endl;
    return;
  }
  int64_t len = 0;
  do {
    const auto nbp = strtoul(itr, &itr, 10);
    const char op = *itr++;

    if(op == 'I' || op == 'M' || op == 'S' || op == '=' || op =='X') {
      len += nbp;
    }
  } while(itr != end);
  std::cout << "seq length " << len << std::endl;
}


template<class Seq_>
struct RawSAMRecord;

template<class Seq_>
struct Trait<RawSAMRecord<Seq_> > {
  using Impl = std::vector<char>;
  using String = char const*;
  using SeqView = Range<String>;
  using Pos = uint32_t;
  using RefId = String;
  using Refs = typename bio::References<std::string, Seq_>;
  using RefPtr = typename Refs::SeqPtr;
};

template<class Seq_>
struct RawSAMRecord : SeqBase<RawSAMRecord<Seq_> > {
  using Buffer = std::vector<char>;
  using String = typename Trait<RawSAMRecord>::String;
  using SeqView = typename Trait<RawSAMRecord>::SeqView;

  explicit RawSAMRecord(Buffer&& line) : line_(std::move(line)) { init(); }

  RawSAMRecord(RawSAMRecord&& other) = default;

  RawSAMRecord(RawSAMRecord const&) = delete;

  RawSAMRecord& operator=(RawSAMRecord const&) = delete;

  RawSAMRecord() = delete;

  Buffer& line() { return line_; }

  Buffer const& line() const { return line_; }

  SeqView seq() const { return SeqView(seq_, qual_ - 1); }

  SeqView qual() const { return SeqView(qual_, optional_ - 1); }

  SeqView cigar() const { return SeqView(cigar_, rnext_ - 1); }

private:
  friend struct SeqBase<RawSAMRecord>;

  String const& _qname() const { return qname_; }

  typename Trait<RawSAMRecord>::RefId _ref_id() const { return rname_; }

  typename Trait<RawSAMRecord>::Pos _ref_begin0() const { return pos0_; }

  bool _isMapped() const { return not (0x4 & flag_); }

  template<class Generator>
  bool _load(Generator& gen) {
    if (gen.load(line_)) {
      init();
      return true;
    }
    return false;
  }

  void init() {
    char* curr = line_.data();
    qname_ = curr;
    curr = strchr(curr, '\t');
    *curr++ = '\0'; // end of QNAME

    flag_ = static_cast<unsigned short>(strtoul(curr, &curr, 10));
    *curr++ = '\0';

    rname_ = curr;
    curr = strchr(curr, '\t');
    *curr++ = '\0'; // end of RNAME

    pos0_ = strtoul(curr, &curr, 10) - 1;
    *curr++ = '\0';

    mapq_ = static_cast<unsigned char>(strtoul(curr, &curr, 10));
    *curr++ = '\0';

    cigar_ = curr;
    curr = strchr(curr, '\t');
    *curr++ = '\0'; // end of CIGAR

    rnext_ = curr;
    curr = strchr(curr, '\t');
    *curr++ = '\0'; // end of RNEXT

    pnext_ = strtoul(curr, &curr, 10);
    *curr++ = '\0';

    tlen_ = strtol(curr, &curr, 10);
    *curr++ = '\0';

    seq_ = curr;
    curr = strchr(curr, '\t');
    *curr++ = '\0'; // end of SEQ

    qual_ = curr;
    curr = strchr(curr, '\t');
    *curr++ = '\0'; // end of QUAL

    optional_ = curr;
  }

  Buffer line_;
  String qname_;
  String rname_;
  String cigar_;
  String rnext_;
  String seq_;
  String qual_;
  String optional_;

  uint32_t pos0_;
  uint32_t pnext_;
  int32_t tlen_;
  unsigned short flag_;
  unsigned char mapq_;
};

template<class Seq_>
void Print(std::ostream& os, RawSAMRecord<Seq_> const& record, bool print_alignment) {
  os << "record: " << record.qname();
  if (record.isMapped()) {
    os << " " << record.ref_id()
    << " " << record.ref_begin0()
      /*
        << " " << getContigName(record.impl(), record.bam())
        << "(" << getContigLength(record.impl(), record.bam()) << ")"
        << " " << record.ref_end0();
       */
            ;
  }
  const auto& seq = record.seq();
  os << " " << seq.size()
  << " " << record.qual().size();
  os << "\n";
  bool valid = true;
  for (auto itr = seq.begin(); itr != seq.end(); ++itr) {
    if (*itr != 'A' and *itr != 'C' and *itr != 'G' and *itr != 'T') {
      os << "WARNING: at " << std::distance(seq.begin(), itr) << " " << *itr << std::endl;
      valid = false;
      break;
    }
  }
  if (valid) {
    os << "valid" << std::endl;
  }
  ProcessCigar(record.cigar().begin(), record.cigar().end());
  if (print_alignment and record.isMapped()) {
    /*
      seqan::Align<Seq_> align;
      //pretty hard to enforce constness with seqan's class
      bamRecordToAlignment(align, const_cast<Seq_&>(*record.ref()), record.impl());
      os << align << '\n';
     */
  }
}

}
}
#endif //BIO_SAMBAM_LOCAL_SAM_H
