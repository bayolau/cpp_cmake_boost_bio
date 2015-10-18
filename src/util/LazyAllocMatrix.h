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
// Created by bayolau on 10/17/15.
//

#ifndef UTIL_LAZZYALLOCMATRIX_H
#define UTIL_LAZZYALLOCMATRIX_H

#include <vector>
#include <iostream>
#include <boost/align/aligned_allocator.hpp>

namespace bayolau {

template<class T, size_t Alignment, bool RowMajor_ = true>
struct LazyAllocMatrix {
  using Container = std::vector<T, boost::alignment::aligned_allocator<T, Alignment>>;
  using value_type = T;
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;
  static const bool RowMajor = RowMajor_;
private:
  static size_t Stride(size_t r, size_t c) {
    return (sizeof(T) * (c * RowMajor + r * (not RowMajor)) / Alignment + 1) * Alignment / sizeof(T);
  }

  static size_t MinSize(size_t r, size_t c) {
    return (c * (not RowMajor) + r * RowMajor) * Stride(r, c);
  }

  size_t Index(size_t r, size_t c) const {
    return (c * (not RowMajor) + r * RowMajor) * stride_ + c * RowMajor + r * (not RowMajor);
  }

  size_t stride_;
  size_t row_;
  size_t col_;
  std::vector<T, boost::alignment::aligned_allocator<T, Alignment>> data_;
public:
  const_iterator begin(size_t stride) const {
    return data_.begin() + stride * stride_;
  }

  const_iterator end(size_t stride) const {
    return this->begin(stride) + col_ * RowMajor + row_ * (not RowMajor);
  }

  iterator begin(size_t stride) {
    return data_.begin() + stride * stride_;
  }

  iterator end(size_t stride) {
    return this->begin(stride) + col_ * RowMajor + row_ * (not RowMajor);
  }

  size_t row() const { return row_; }

  size_t col() const { return col_; }

  void resize(size_t r, size_t c) {
    stride_ = Stride(r, c);
    row_ = r;
    col_ = c;
    if (data_.size() < MinSize(r, c)) data_.resize(MinSize(r, c));
  }

  // set each run of the major dimension as the respective element of the list
  template<class List>
  void SetMajor(List const& list) {
    auto itr = begin(list);
    for (size_t shift = 0; shift < data_.size() && itr != end(list); ++itr) {
      for (size_t stride_end = shift + stride_; shift < stride_end; ++shift) {
        data_[shift] = *itr;
      }
    }
  }

  // set the index'th major row to val
  void SetMajor(size_t index, T const& val) {
    for (size_t idx = index * stride_, end = (index + 1) * stride_; idx != end; ++idx) {
      data_[idx] = val;
    }
  }

  LazyAllocMatrix() : LazyAllocMatrix(0, 0) { };

  LazyAllocMatrix(size_t r, size_t c) : stride_(Stride(r, c)), row_(r), col_(c), data_(MinSize(r, c)) { }

  LazyAllocMatrix(size_t r, size_t c, T const& val) : stride_(Stride(r, c)), row_(r), col_(c),
                                                      data_(MinSize(r, c), val) { }

  T& operator()(size_t r, size_t c) {
    return data_[Index(r, c)];
  }

  T const& operator()(size_t r, size_t c) const {
    return data_[Index(r, c)];
  }
};

template<class T, size_t Alignment, bool RowMajor>
std::ostream& operator<<(std::ostream& os, LazyAllocMatrix<T, Alignment, RowMajor> const& other) {
  for (size_t rr = 0; rr < other.row(); ++rr) {
    for (size_t cc = 0; cc < other.col(); ++cc) {
      os << other(rr, cc) << ' ';
    }
    os << ':' << rr;
    os << '\n';
  }
  return os;
}

}
#endif //UTIL_LAZZYALLOCMATRIX_H
