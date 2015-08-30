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
// Created by laub2 on 8/27/15.
//

#include <utility>

#include "bio/Forward.h"
#include "bio/References.h"
#include "bio/seq/SeqBase.h"
#include "util/StdinByLine.h"

#ifndef BIO_SAMBAM_LOCAL_SAM_H
#define BIO_SAMBAM_LOCAL_SAM_H
namespace bayolau {
namespace bio {

template<class Seq_>
struct RawSAMRecord;

template<class Seq_>
struct Trait<RawSAMRecord<Seq_>> {
  using Seq = Seq_;
  using Impl = std::vector<char>;
  using String = char const*;
  using Pos = uint32_t;
  using RefId = String;
  using Refs = typename bio::References<String, Seq>;
  using RefPtr = typename bio::References<String, Seq>::SeqPtr;
};

struct RawSAMHeader {
  using Line = std::vector<char>;

  template<class T>
  void push_back(T&& line) { lines_.push_back(std::forward<T>(line)); }

private:
  std::list<Line> lines_;
};

template<class Seq_>
struct RawSAMRecord : SeqBase<RawSAMRecord<Seq_> > {
  using Line = std::vector<char>;
  using String = typename Trait<RawSAMRecord>::String;

  RawSAMRecord(Line&& line) : line_(std::move(line)) { init(); }

  Line& line() { return line_; }

  Line const& line() const { return line_; }

private:
  friend struct SeqBase<RawSAMRecord>;

  String const& _qname() const { return qname_; }

  bool _isMapped() const { return 0x4 & flag_; }

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

    pos_ = strtoul(curr, &curr, 10);
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

  Line line_;
  String qname_;
  String rname_;
  String cigar_;
  String rnext_;
  String seq_;
  String qual_;
  String optional_;

  uint32_t pos_;
  uint32_t pnext_;
  int32_t tlen_;
  unsigned short flag_;
  unsigned char mapq_;
};


template<class Seq_>
struct StdinSAMReader {
  using Seq = Seq_;
  using Refs = References<char const* const, Seq>;
  using value_type = RawSAMRecord<Seq>;
  using pointer = std::unique_ptr<value_type>;

  StdinSAMReader(const std::string& fai) : header_(), refs_(fai) { }

  StdinSAMReader() = delete;

  StdinSAMReader(const StdinSAMReader&) = delete;

  StdinSAMReader& operator=(const StdinSAMReader&) = delete;

  bool load(typename value_type::Line& buffer) {
    while (bayolau::StdinByLine::Read(buffer)) {
      if (buffer.size() > 1) { // longer than null
        if (buffer[0] != '@') {
          return true;
        }
        else {
          header_.push_back(buffer); // make a copy into header, this is rare
        }
      }
    }
    return false;
  }

  pointer Next() {
    typename value_type::Line buffer;
    pointer ret;
    return load(buffer) ? pointer(new value_type(std::move(buffer))) : nullptr;
  }

private:
  RawSAMHeader header_;
  Refs refs_;
};

template<class Seq_>
void Print(std::ostream& os, RawSAMRecord<Seq_> const& record, bool print_alignment) {
  os << "record: " << record.qname();
  /*
  if (record.isMapped()) {
    os << " " << getContigName(record.impl(), record.bam())
    << " " << record.ref_id()
    << "(" << getContigLength(record.impl(), record.bam()) << ")"
    << " " << record.ref_begin0()
    << " " << record.ref_end0();
  }
   */
  os << "\n";
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
