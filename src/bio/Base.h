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

#include <array>

namespace bayolau {

struct Base {
  // to do: make it use static member but need to change build system
  static char complement(char other) {
    return instance().rc_mapper_[other];
  }

  static char to_val(char other) {
    return instance().to_val_[other];
  }

  static char to_char(char other) {
    return instance().to_char_[other];
  }

  static Base const& instance() {
    static Base instance;
    return instance;
  }

  enum BP_CODE {
    BP_A = 0,
    BP_C = 1,
    BP_G = 2,
    BP_T = 3,
    BP_GAP = 4
  };

private:
  std::array<char, 256> rc_mapper_;
  std::array<char, 256> to_char_;
  std::array<unsigned char, 256> to_val_;

  Base() {
    rc_mapper_.fill(' ');
    rc_mapper_[BP_A] = BP_T;
    rc_mapper_[BP_C] = BP_G;
    rc_mapper_[BP_G] = BP_C;
    rc_mapper_[BP_T] = BP_A;
    rc_mapper_[BP_GAP] = BP_GAP;

    rc_mapper_['A'] = 'T';
    rc_mapper_['a'] = 'T';

    rc_mapper_['C'] = 'G';
    rc_mapper_['c'] = 'G';

    rc_mapper_['G'] = 'C';
    rc_mapper_['g'] = 'C';

    rc_mapper_['T'] = 'A';
    rc_mapper_['t'] = 'A';
    rc_mapper_[' '] = ' ';

    to_val_.fill(255); // crash delibrately
    //preserve values
    to_val_[BP_A] = BP_A;
    to_val_[BP_C] = BP_C;
    to_val_[BP_G] = BP_G;
    to_val_[BP_T] = BP_T;
    to_val_[BP_GAP] = BP_GAP;

    //ascii
    to_val_['A'] = BP_A;
    to_val_['a'] = BP_A;

    to_val_['C'] = BP_C;
    to_val_['c'] = BP_C;

    to_val_['G'] = BP_G;
    to_val_['g'] = BP_G;

    to_val_['T'] = BP_T;
    to_val_['t'] = BP_T;

    // space
    to_val_[' '] = BP_GAP;


    to_char_.fill('?'); // crash delibrately
    //preserve values
    to_char_[BP_A] = 'A';
    to_char_[BP_C] = 'C';
    to_char_[BP_G] = 'G';
    to_char_[BP_T] = 'T';
    to_char_[BP_GAP] = ' ';

    to_char_['A'] = 'A';
    to_char_['a'] = 'A';

    to_char_['C'] = 'C';
    to_char_['c'] = 'C';

    to_char_['G'] = 'G';
    to_char_['g'] = 'G';

    to_char_['T'] = 'T';
    to_char_['t'] = 'T';

    // space
    to_char_[' '] = ' ';
  }
};

}
#endif //BASE_H
