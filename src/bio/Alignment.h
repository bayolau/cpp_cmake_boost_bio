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

#ifndef BIO_ALIGNMENT_H
#define BIO_ALIGNMENT_H

#include <type_traits>

namespace bayolau {
namespace bio {
namespace alignment {

template<class Int>
struct Element {
  static_assert(std::is_integral<Int>::value, "must be integral");
  enum {
    Org = 0, Diag = 1, Horizontal = 2, Vertical = 3, NumPrev = 4
  };

  void set(Int score, Int dir) { data_ = score * NumPrev + dir; }

  Int dir() const { return data_ % NumPrev; }

  Int score() const { return data_ / NumPrev; }

  friend std::ostream& operator<<(std::ostream& os, Element const& other) {
    switch (other.dir()) {
      case Org:
        os << 'o';
        break;
      case Diag:
        os << '\\';
        break;
      case Horizontal:
        os << '-';
        break;
      case Vertical:
        os << '|';
        break;
    }
    auto score = other.score();
    os << std::setw(2) << score;
    return os;
  }

private:
  Int data_;
};

template<class Matrix>
std::pair<size_t, size_t> TraceFront(size_t row, size_t col, Matrix const& matrix) {
  while (row > 1 and col > 1) {
    switch (matrix(row, col).dir()) {
      case Matrix::value_type::Org:
        return std::make_pair(row, col);
      case Matrix::value_type::Diag:
        --row;
        --col;
        break;
      case Matrix::value_type::Horizontal:
        --col;
        break;
      case Matrix::value_type::Vertical:
        --row;
        break;
    }
  }
  return std::make_pair(row, col);
}

}
}
}
#endif //BIO_ALIGNMENT_H
