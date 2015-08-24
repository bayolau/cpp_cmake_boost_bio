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
// Created by Bayo Lau on 8/15/15.
//

#ifndef BIO_REFERENCES_H
#define BIO_REFERENCES_H

#include <map>
#include "Fai.h"
#include "util/Logging.h"

namespace bayolau {
namespace bio {

template<class String_, class Seq_>
struct References {
  using Seq = Seq_;
  using SeqPtr = std::shared_ptr<const Seq_>; // shared_ptr required by erase()

  explicit References(const std::string& filename) : handler_(filename), map_() { }

  References(const References& filename) = delete;

  References& operator=(const References& filename) = delete;

  References() = delete;

  /**
   * @brief make sure a reference is in cache, then return a pointer
   * \warning not thread safe
   * @param[in] name sequence name
   * @return a pointer to sequence, null if it cannot be found
   */
  SeqPtr operator[](const String_& name) const {
    // length() for seqan's string can blow up for some reason
    if (std::distance(begin(name), end(name)) < 1) return nullptr;
    auto itr = map_.find(name);
    if (itr == map_.end()) {
      LOG(info) << "loading " << std::string(begin(name), end(name));
      SeqPtr ptr(handler_.get<Seq>(name));
      return map_.insert(std::make_pair(name, ptr)).first->second;
    }
    return itr->second;
  }

  /**
   * @brief remove reference of name, deallocation is deffered until shared_ptr is deallocated
   * \warning not thread safe
   * @param[in] name sequence name
   */
  void erase(const String_& name) {
    auto itr = map_.find(name);
    if (itr != map_.end()) map_.erase(itr);
  }

private:
  const Fai handler_;
  mutable std::map<String_, SeqPtr> map_;
};

}
}
#endif //REFERENCES_H
