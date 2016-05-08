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
// Created by bayo on 3/16/16.
//

#ifndef UTIL_PERIODICADAPTOR_H
#define UTIL_PERIODICADAPTOR_H

#include <iostream>

namespace bayolau {
template<class Container>
class PeriodicAdaptor {
  Container const& container_;
  const size_t total_size_;

public:
  struct const_iterator {
    using ci = typename Container::const_iterator;

    const_iterator(ci i, Container const& r) : reference_(r), itr_(wrap(i)) { };

    typename Container::const_reference operator*() const {
      return *itr_;
    }

    const_iterator& operator++() {
      itr_ = wrap(itr_ + 1);
      return *this;
    }

    const_iterator operator+(int64_t shift) const {
      return const_iterator(wrap(itr_ + shift), reference_);
    }

  private:
    const Container& reference_;
    ci itr_;

    ci wrap(ci itr) const {
      return reference_.begin() + std::distance(reference_.begin(), itr) % reference_.size();
    }
  };

  PeriodicAdaptor(Container const& container, size_t total_size) : container_(container), total_size_(total_size) { }

  size_t size() const { return total_size_; }

  auto operator[](size_t idx) const -> decltype(container_[0]) {
    return container_[idx % container_.size()];
  }

  const_iterator begin() const { return const_iterator(container_.begin(), container_); }

  friend std::ostream& operator<<(std::ostream& os, PeriodicAdaptor const& in) {
    for (size_t idx = 0; idx < in.size(); ++idx) {
      os << in[idx];
    }
    return os;
  }
};

template<class Container>
size_t length(PeriodicAdaptor<Container> const& in) {
  return in.size();
}
}
#endif //PERIODICADAPTOR_H
