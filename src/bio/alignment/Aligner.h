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
// Created by bayo on 10/26/15.
//

#ifndef BIO_ALIGNMENT_ALIGNER_H
#define BIO_ALIGNMENT_ALIGNER_H

#include <string>
#include "bio/alignment/Alignment.h"
#include "util/LazyAllocMatrix.h"

namespace bayolau {
namespace bio {
namespace alignment {

inline size_t length(std::string const& in) {
  return in.size();
}

struct Aligner {
  using FrontBackScore = std::tuple<size_t, size_t, int>;

  template<class SeqR, class SeqC, class Option>
  auto operator()(SeqR const& sr, SeqC const& sc, Option const& po)
    -> Alignment< typename std::remove_cv<typename std::remove_reference<decltype(sr[0])>::type>::type
                , typename std::remove_cv<typename std::remove_reference<decltype(sc[0])>::type>::type
                > {
    return Process(matrix_, sr, sc, po);
  }

private:
  using Element = bayolau::bio::alignment::Element<int>;
  using Matrix = bayolau::LazyAllocMatrix<Element, 16, true>;

  Matrix matrix_;

  template<class SeqR, class SeqC, class Option>
  auto Process(Matrix& matrix, SeqR const& sr, SeqC const& sc, Option const& po)
    -> Alignment< typename std::remove_cv<typename std::remove_reference<decltype(sr[0])>::type>::type
                , typename std::remove_cv<typename std::remove_reference<decltype(sc[0])>::type>::type
                > {
    const int gap = - int(po.gap_penalty());
    const int match = int(po.match_bonus());
    const int mismatch = - int(po.mis_penalty());

    matrix.resize(length(sr) + 1, length(sc) + 1);

    { // first row, row major
      auto itr = matrix.begin(0);
      (*itr++).set(0, Element::Org);
      for (size_t cc = 1, ce = matrix.col(); cc != ce; ++cc) { (*itr++).set(int(cc)*gap, Element::Horizontal); }
    }

    for (size_t rr = 1; rr < matrix.row(); ++rr) { // for each row
      auto itr = matrix.begin(rr); // data of this row

      (*itr++).set(int(rr)*gap, Element::Vertical); // first column
      int last_score = int(rr)*gap; //last score from the same row

      const auto bi = sr[rr - 1];
      auto itr_last_row = matrix.begin(rr - 1); // data of last row
      for (size_t cc = 1; cc < matrix.col(); ++cc, ++itr) {
        const auto s = (bi == sc[cc - 1]) ? match : mismatch;
        int dir = Element::Diag;
        int score = (*itr_last_row++).score() + s; // (rr-1, cc-1)
        int tmp = itr_last_row->score() + gap; // (rr-1,cc)
        if (tmp > score) {
          score = tmp;
          dir = Element::Vertical;
        }
        tmp = last_score + gap; // (rr, cc-1)
        if (tmp > score) {
          score = tmp;
          dir = Element::Horizontal;
        }

        itr->set(score, dir);
        last_score = score;
      }
    }
    using RT = typename std::remove_cv<typename std::remove_reference<decltype(sr[0])>::type>::type;
    using CT = typename std::remove_cv<typename std::remove_reference<decltype(sc[0])>::type>::type;
    return RetrieveAlignment<RT,CT>(sr.begin(), sc.begin(), matrix.row() - 1, matrix.col() - 1, matrix);
  }
};
}
}
}
#endif //BIO_ALIGNMENT_ALIGNER_H
