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

#ifndef UTIL_STATSLOGGER_H
#define UTIL_STATSLOGGER_H

namespace bayolau {

template<class ENUM_WITH_MAP>
class StatsLogger {
  using ENUM = typename ENUM_WITH_MAP::ENUM;
  std::array<size_t, static_cast<int>(ENUM::NUM_FIELDS)> data_;
public:
  StatsLogger() : data_() { }

  ENUM push(ENUM e) {
    ++data_[static_cast<int>(e)];
    return e;
  }

  StatsLogger& operator+=(StatsLogger const& other) {
    std::transform(data_.begin(), data_.end(), other.data_.begin(), data_.begin(), std::plus<size_t>());
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, StatsLogger const& in) {
    os << __PRETTY_FUNCTION__ << ":";
    for (size_t idx = 0; idx < in.data_.size(); ++idx) {
      os << " " << ENUM_WITH_MAP::to_string(idx) << " " << in.data_[idx];
    }
    return os;
  }
};

}


#endif //UTIL_STATSLOGGER_H
