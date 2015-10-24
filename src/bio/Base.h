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
// Created by bayo on 10/23/15.
//

#ifndef BASE_H
#define BASE_H
namespace bayolau {

struct Base {
  // to do: make it use static member but need to change build system
  static char complement(char other) {
    return instance().rc_mapper_[other];
  }

  static char to_val(char other) {
    return instance().to_val_[other];
  }

  static Base const& instance() {
    static Base instance;
    return instance;
  }

private:
  std::array<char, 256> rc_mapper_;
  std::array<unsigned char, 256> to_val_;

  Base() {
    rc_mapper_.fill(' ');
    rc_mapper_[0] = 3;
    rc_mapper_[1] = 2;
    rc_mapper_[2] = 1;
    rc_mapper_[3] = 0;

    rc_mapper_['A'] = 'T';
    rc_mapper_['a'] = 'T';

    rc_mapper_['C'] = 'G';
    rc_mapper_['c'] = 'G';

    rc_mapper_['G'] = 'C';
    rc_mapper_['g'] = 'C';

    rc_mapper_['T'] = 'A';
    rc_mapper_['t'] = 'A';

    to_val_.fill(255); // crash delibrately
    //preserve values
    to_val_[0] = 0;
    to_val_[1] = 1;
    to_val_[2] = 2;
    to_val_[3] = 3;

    //ascii
    to_val_['A'] = 0;
    to_val_['a'] = 0;

    to_val_['C'] = 1;
    to_val_['c'] = 1;

    to_val_['G'] = 2;
    to_val_['g'] = 2;

    to_val_['T'] = 3;
    to_val_['t'] = 3;
  }
};

}
#endif //BASE_H
