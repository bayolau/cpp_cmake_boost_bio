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
// Created by bayo on 10/22/15.
//

#ifndef BIO_SEQ_BASEQUAL_H
#define BIO_SEQ_BASEQUAL_H

#include <vector>
#include "bio/Base.h"

namespace bayolau {
namespace bio {

template<class M>
struct Meta {
  Meta() = default;

  Meta(M m) : meta_(m) { }

private:
  M meta_;
};

// empty baseclass optimization
template<>
struct Meta<void> {
};

template<class B, class M>
struct BaseMeta : Meta<M> {
  BaseMeta() = default;

  template<class MM = M>
  BaseMeta(B b, typename std::enable_if<is_void<MM>::value, M>::type* dummy = nullptr) : base_(Base::to_val(b)) { }

  template<class MM = M>
  BaseMeta(B b, MM m, typename std::enable_if<!is_void<MM>::value, M>::type* dummy = nullptr) : Meta<M>(m), base_(Base::to_val(b)) { }

  void rc() {
    base_ = Base::complement(base_);
  }

  operator B() const { return base_; }

private:
  B base_;
};

static_assert(sizeof(BaseMeta<char, void>) == 1, "empty base class optimization is required. Don't mess with my code.");
static_assert(sizeof(BaseMeta<char, char>) == 2, "byte pack required. Don't mess with my code.");
static_assert(sizeof(BaseMeta<char, std::array<char, 3>>) == 4, "byte pack required. Don't mess with my code.");
static_assert(sizeof(BaseMeta<char, std::array<char, 7>>) == 8, "byte pack required. Don't mess with my code.");

// to do: complete crtp when have time, busy with application now
template<class B, class M>
struct SeqMeta {
  using Container = std::vector<BaseMeta<B, M>>;

  SeqMeta() = default;

  SeqMeta(typename Container::const_iterator i, typename Container::const_iterator e) : data_(i,e) { }

  template<class ItrS, class ItrM = void*, class MM = M>
  SeqMeta(ItrS s_itr, ItrS s_end, ItrM m_itr = nullptr, ItrM m_end = nullptr,
          typename std::enable_if<is_void<MM>::value, M>::type* dummy = nullptr) {
    data_.reserve(std::distance(s_itr, s_end));
    for (; s_itr != s_end; ++s_itr) {
      data_.emplace_back(*s_itr);
    }
  }

  template<class ItrS, class ItrM, class MM = M>
  SeqMeta(ItrS s_itr, ItrS s_end, ItrM m_itr, ItrM m_end,
          typename std::enable_if<!is_void<MM>::value, M>::type* dummy = nullptr) {
    data_.reserve(std::distance(s_itr, s_end));
    if (m_itr != m_end) {
      for (; s_itr != s_end; ++s_itr, ++m_itr) {
        data_.emplace_back(*s_itr, *m_itr);
      }
    }
    else {
      for (; s_itr != s_end; ++s_itr) {
        data_.emplace_back(*s_itr, 0);
      }
    }
  }

  typename Container::reference operator[](size_t index) {
    return data_[index];
  }

  typename Container::const_reference operator[](size_t index) const {
    return data_[index];
  }

  size_t size() const {
    return data_.size();
  }

  void rc() {
    auto front = data_.begin(), back = data_.end() - 1;
    for (; std::distance(front, back) > 0; ++front, --back) {
      front->rc();
      back->rc();
      swap(*front, *back);
    }
    if(front == back) {
      front->rc();
    }
  }

  typename Container::const_iterator begin() const {
    return data_.begin();
  }

  typename Container::const_iterator end() const {
    return data_.end();
  }

private:
  Container data_;
};


template<class B, class M>
inline size_t length(SeqMeta<B, M> const& entry) {
  return entry.size();
}

template<class B, class M>
inline void reverseComplement(SeqMeta<B, M>& entry) {
  entry.rc();
}

}
}
#endif //BIO_SEQ_BASEQUAL_H
