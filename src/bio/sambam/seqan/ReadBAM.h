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
// Created by bayolau on 8/30/15.
//

#ifndef BIO_SAMBAM_SEQAN_READBAM_H
#define BIO_SAMBAM_SEQAN_READBAM_H
namespace bayolau {
namespace bio {

#include "bio/Forward.h"

template<class Record_>
struct BamReader {
  using Refs = typename Trait<Record_>::Refs;
  using value_type = Record_;
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
    return pointer(new value_type(in_, refs_));
  }

  seqan::BamFileIn& impl() { return in_; }

private:
  seqan::BamFileIn in_;
  seqan::BamHeader header_;
  Refs refs_;
};


}
}
#endif //BIO_SAMBAM_SEQAN_READBAM_H
