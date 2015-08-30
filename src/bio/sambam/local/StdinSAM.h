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

#ifndef BIO_SAMBAM_LOCAL_STDINSAM_H
#define BIO_SAMBAM_LOCAL_STDINSAM_H

#include "bio/Forward.h"
#include "util/StdinByLine.h"

namespace bayolau {
namespace bio {

template<class Buffer_>
struct RawSAMHeader {
  using Buffer = Buffer_;

  template<class T>
  void push_back(T&& buffer) { buffers_.push_back(std::forward<T>(buffer)); }

private:
  std::list<Buffer> buffers_;
};


template<class Record_>
struct StdinSAM {
  using value_type = Record_;
  using Refs = typename Trait<Record_>::Refs;
  using Buffer = typename value_type::Buffer;
  using pointer = std::unique_ptr<value_type>;

  explicit StdinSAM(const std::string& fai) : header_(), refs_(fai) { }

  StdinSAM(StdinSAM&& other) = default;

  StdinSAM() = delete;

  StdinSAM(const StdinSAM&) = delete;

  StdinSAM& operator=(const StdinSAM&) = delete;

  bool load(Buffer& buffer) {
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
    Buffer buffer;
    return load(buffer) ? pointer(new value_type(std::move(buffer))) : nullptr;
  }

private:
  RawSAMHeader<Buffer> header_;
  Refs refs_;
};

}
}
#endif //BIO_SAMBAM_LOCAL_STDINSAM_H
