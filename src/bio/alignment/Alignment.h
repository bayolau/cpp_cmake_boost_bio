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

#ifndef BIO_ALIGNMENT_ALIGNMENT_H
#define BIO_ALIGNMENT_ALIGNMENT_H

#include <type_traits>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "bio/Base.h"

namespace bayolau {
namespace bio {
namespace alignment {

struct AlignmentOptions {
  template<class Option>
  explicit AlignmentOptions(Option const& o) : r_gap_penalty_(o.r_gap_penalty()), c_gap_penalty_(o.c_gap_penalty()),
          r_flank_gap_penalty_(o.r_flank_gap_penalty()), match_bonus_(o.match_bonus()), mis_penalty_(o.mis_penalty()) { }

  AlignmentOptions(int a, int b, int c, int d, int e) : r_gap_penalty_(a), c_gap_penalty_(b), r_flank_gap_penalty_(c), match_bonus_(d), mis_penalty_(e) { }

  int r_gap_penalty() const { return r_gap_penalty_; }

  int c_gap_penalty() const { return c_gap_penalty_; }

  int r_flank_gap_penalty() const { return r_flank_gap_penalty_; }

  int match_bonus() const { return match_bonus_; }

  int mis_penalty() const { return mis_penalty_; }

private:
  int r_gap_penalty_;
  int c_gap_penalty_;
  int r_flank_gap_penalty_;
  int match_bonus_;
  int mis_penalty_;
};


template<class Int>
struct Element {
  static_assert(std::is_integral<Int>::value, "must be integral");
  enum {
    Org = 0, Diag = 1, Horizontal = 2, Vertical = 3, NumPrev = 4
  };

  void set(Int score, Int dir) {
    score_ = score;
    dir_ = dir;
  }

  char dir() const { return dir_; }

  Int score() const { return score_; }

  friend std::ostream& operator<<(std::ostream& os, Element const& other) {
    switch (other.dir()) {
      case Org:
        os << 'o';
        break;
      case Diag:
        os << '\\';
        break;
      case Horizontal:
        os << '=';
        break;
      case Vertical:
        os << '|';
        break;
      default:
        os << '?';
        break;
    }
    os << std::setfill('_') << std::setw(3) << other.score();
    return os;
  }

private:
  Int score_;
  char dir_;
};

template<class Matrix>
std::pair<size_t, size_t> TraceFront(size_t row, size_t col, Matrix const& matrix) {
  std::pair<size_t, size_t> ret(row, col);
  while (row >= 1 and col >= 1) {
    switch (matrix(row, col).dir()) {
      case Matrix::value_type::Org:
        return ret;
      case Matrix::value_type::Diag:
        ret.first = row;
        ret.second = col;
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
  return ret;
}

template<class RT_, class CT_>
struct Alignment {
  using RT = RT_;
  using CT = CT_;

private:
  // ugly and not strictly safe but this is private, and is a quick way to be compatible with ALLPATH's code
  struct value_type : public std::pair<RT, CT> {
    value_type(RT a, CT b) : std::pair<RT, CT>(a, b) { }

    bool gap_a() const {
      return Base::to_val(this->first) == Base::BP_GAP;
    }

    bool gap_b() const {
      return Base::to_val(this->second) == Base::BP_GAP;
    }

    RT base_a() const {
      return this->first;
    }

    CT base_b() const {
      return this->second;
    }
  };

public:
  using Container = std::vector<value_type>;

  void emplace_back(RT r, CT c) {
    c_.emplace_back(r, c);
  }

  void reverse() {
    std::reverse(c_.begin(), c_.end());
  }

  friend std::ostream& operator<<(std::ostream& os, const Alignment& other) {
    os << "a ";
    for (const auto& entry: other.c_) { os << Base::to_char(entry.first); }
    os << "\n";
    os << "b ";
    for (const auto& entry: other.c_) { os << Base::to_char(entry.second); }
    os << "\n";
    return os;
  }

  size_t size() const { return c_.size(); }

  typename Container::const_iterator cbegin() const { return c_.cbegin(); }

  typename Container::const_iterator cend() const { return c_.cend(); }

  value_type const& operator[](size_t idx) const {
    return c_[idx];
  }

  // these are to fool code written for allpaths
  Alignment const& bases() const { return *this; }

  size_t edit_distance() const { // quick and dirty hack, need to properly use matrix transversal
    size_t ret = 0;
    for (const auto& entry: c_) {
      ret += static_cast<size_t>(Base::to_char(entry.first) != Base::to_char(entry.second));
    }
    return ret;
  }

  std::string cigar(std::vector<char> const& lookup) const { // quick and dirty, return the cigar of RT aligned to CT, with lookup
    std::cout << *this << std::endl;
    std::string ret;
    ret.reserve(c_.size());
    enum {INS = 0, DEL, MIS, MAT, UNDEFINED};
    int state = UNDEFINED;
    int length = 0;
    for(auto const& entry: c_) {
      int new_state = UNDEFINED;
      if ( entry.gap_a() ) { new_state = DEL; }
      else if ( entry.gap_b() ) { new_state = INS; }
      else if (entry.base_a() == entry.base_b()) { new_state = MAT; }
      else { new_state = MIS; }
      if (new_state == state) {
        ++length;
      }
      else {
        if (state != UNDEFINED) {
          ret+=std::to_string(length);
          ret+=lookup[state];
        }
        state = new_state;
        length = 1;
      }
    }
    if (state != UNDEFINED) {
      ret+=std::to_string(length);
      ret+=lookup[state];
    }
    std::cout << ret << std::endl;
    return ret;
  }

private:
  Container c_;
};

template<class RT, class CT, class RItr, class CItr, class Matrix>
Alignment<RT, CT> RetrieveAlignment(RItr ritr, CItr citr, int64_t row, int64_t col, Matrix const& matrix) {
  Alignment<RT, CT> ret;
  while (row != 0 or col != 0) {
    switch (matrix(row, col).dir()) {
      case Matrix::value_type::Org:
        ret.reverse();
        return ret;
      case Matrix::value_type::Diag:
        --row;
        --col;
        ret.emplace_back(*(ritr + row), *(citr + col));
        break;
      case Matrix::value_type::Horizontal:
        --col;
        ret.emplace_back(RT(Base::BP_GAP), *(citr + col));
        break;
      case Matrix::value_type::Vertical:
        --row;
        ret.emplace_back(*(ritr + row), CT(Base::BP_GAP));
        break;
      default:
        std::cout << "uh oh " << int(matrix(row, col).dir()) << std::endl;
        ret.reverse();
        return ret;
    }
  }
  ret.reverse();
  return ret;
}

}
}
}
#endif //BIO_ALIGNMENT_ALIGNMENT_H
