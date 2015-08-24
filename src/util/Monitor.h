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
// Created by Bayo Lau on 8/23/15.
//

#ifndef UTIL_MONITOR_H
#define UTIL_MONITOR_H

#include <chrono>
#include <iostream>
#include <boost/preprocessor/stringize.hpp>

namespace bayolau {
struct Timer {
  using clock_type = std::chrono::steady_clock;
  using time_point = clock_type::time_point;

  Timer(char const* const marker, char const* const loc) : start_(clock_type::now()), marker_(marker), loc_(loc) { }

  ~Timer() {
    std::cerr << std::chrono::duration<double, std::milli>(clock_type::now() - start_).count() << "ms taken in " <<
    marker_ << " @ " << loc_ << std::endl;
  }

private:
  const time_point start_;
  char const* const marker_;
  char const* const loc_;
};
}

#define TIME_SCOPE bayolau::Timer BAYOLAU_MONITOR_TIMER( __PRETTY_FUNCTION__, __FILE__  " : " BOOST_PP_STRINGIZE(__LINE__) );

#endif //UTIL_MONITOR_H
