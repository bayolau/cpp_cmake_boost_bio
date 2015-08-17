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
// Created by Bayo Lau on 8/17/15.
//

#ifndef BIO_LOCUSWINDOW_H
#define BIO_LOCUSWINDOW_H

#include <iostream>

namespace bayolau {
namespace bio {
template<class Generator>
struct LocusWindow {
  using const_pointer = typename Generator::const_pointer;
  using value_type = typename Generator::value_type;

  explicit LocusWindow(Generator& gen) : gen_(gen), next_(gen_.Next()), list_() { LoadOnFirst(); }

  template<class Visitor>
  void Apply(Visitor& visitor) const {
    visitor(list_.begin(), list_.end());
  }

  explicit operator bool() const { return not list_.empty() or next_; }

  LocusWindow& operator++() {
    if (not list_.empty()) { list_.pop_front(); }
    LoadOnFirst();
    return *this;
  }

private:
  Generator& gen_;
  const_pointer next_;
  std::list<const_pointer> list_;

  void LoadOnFirst() {
    if (list_.empty()) FeedList();

    if (not list_.empty()) {
      const auto& locus = list_.front()->locus();
      while (next_) {
        const auto& next_locus = next_->locus();
        if (next_locus.ref() == locus.ref() and next_locus.beg() < locus.end()) {
          FeedList();
        }
        else {
          break;
        }
      }
    }
  }

  void FeedList() {
    if (next_) {
      list_.push_back(std::move(next_));
      next_ = gen_.Next();
    }
  }
};
}
}
#endif //BIO_LOCUSWINDOW_H
