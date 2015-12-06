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
// Created by bayo on 10/13/15.
//

#ifndef UTIL_LAZYALLOCVECTOR_H
#define UTIL_LAZYALLOCVECTOR_H

#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>

template<class T>
struct LazyAllocVector {
  using value_type = T;
  using reference = value_type&;
  using const_reference = value_type const&;
  using Container = std::vector<value_type>;
  using const_iterator = typename Container::const_iterator;
  using iterator = typename Container::iterator;

  size_t size() const { return size_; }

  reference operator[](size_t ii) { return container_[ii]; };

  const_reference operator[](size_t ii) const { return container_[ii]; };

  void clear() { size_ = 0; }

  void resize(size_t new_size) {
    if (new_size > container_.size()) {
      container_.resize(new_size);
    }
    size_ = new_size;
  }

  void push_back(value_type&& other) {
    if (size_ < container_.size()) {
      swap(container_[size_++], other);
    }
    else {
      container_.push_back(other);
      size_ = container_.size();
    }
  }

  void push_back(const value_type& other) {
    if (size_ < container_.size()) {
      container_[size_++] = other;
    }
    else {
      container_.push_back(other);
      size_ = container_.size();
    }
  }

  template<class... Args>
  void emplace_back(Args&& ... args) {
    if (size_ < container_.size()) {
      container_[size_++] = value_type(std::forward<Args>(args)...);
    }
    else {
      container_.emplace_back(std::forward<Args>(args)...);
      size_ = container_.size();
    }
  }

  const_iterator begin() const {
    return container_.begin();
  }
  iterator begin() {
    return container_.begin();
  }

  const_iterator end() const {
    return this->begin() + size_;
  }
  iterator end() {
    return this->begin() + size_;
  }

  const_reference back() const {
    return container_[size_-1];
  }

  reference back() {
    return container_[size_-1];
  }

  template<class Itr>
  void append(Itr b, Itr e) {
    while( b != e && size_ < container_.size() ) {
      swap(container_[size_++], *b);
    }
    if(b != e) {
      container_.insert(container_.end(), b, e);
    }
  }

private:
  Container container_;
  size_t size_;
};


#endif //UTIL_LAZYALLOCVECTOR_H
