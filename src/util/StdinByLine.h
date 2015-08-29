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
// Created by laub2 on 8/27/15.
//

#ifndef UTIL_STDINBYLINE_H
#define UTIL_STDINBYLINE_H
namespace bayolau {

//leaving room for further optimization with a stateful singleton (it's in my head)

//this is obvious not thread safe

struct StdinByLine {
  static const size_t MIN_BUFFER_SIZE = 1000;

  static bool Read(std::vector<char>& buffer) {
    if (buffer.size() < Instance().max_length) buffer.resize(Instance().max_length);
    size_t old_size = 0;
    buffer.back() = '\n';
    char* ret = std::fgets(buffer.data(), buffer.size(), stdin);
    if (ret == NULL) return false;
    while (ret != NULL and buffer.back() == '\0') {
      old_size = buffer.size();
      buffer.resize(old_size * 2);
      buffer.back() = '\n';
      ret = std::fgets(buffer.data() + old_size - 1, old_size + 1, stdin);
    }
    Instance().max_length = std::max(Instance().max_length, buffer.size());
    return true;
  }

  StdinByLine(StdinByLine const& other) = delete;

  StdinByLine& operator=(StdinByLine const& other) = delete;

private:
  static StdinByLine& Instance() {
    static StdinByLine instance;
    return instance;
  }

  StdinByLine() : max_length(MIN_BUFFER_SIZE) { };

  size_t max_length;
};
}
#endif //STDINBYLINE_H
