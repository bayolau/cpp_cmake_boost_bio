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
// Created by laub2 on 8/24/15.
//

#ifndef SEQ_SEQBASE_H
#define SEQ_SEQBASE_H

#include "bio/Forward.h"

namespace bayolau {
namespace bio {

template<class Derived_>
struct SeqBase {
  using Derived = Derived_;
  using Locus = typename Trait<Derived>::Locus;
  using Pos = typename Locus::Pos;
  using RefId = typename Locus::Ref;
  using RefPtr = typename Trait<Derived>::RefPtr;

  template<class Generator>
  bool load(Generator& gen) { return derived()._load(gen); }

  bool isMapped() const { return derived()._isMapped(); }

  Pos size() const { return derived()._size(); }

  Locus locus() const { return derived()._locus(); }

  RefId ref_id() const { return derived()._ref_id(); }

  Pos ref_begin0() const { return derived()._ref_begin0(); }

  Pos ref_end0() const { return derived()._ref_end0(); }

  RefPtr ref() const { return derived()._ref(); }

  // this is temporary
  typename Trait<Derived>::Impl const& impl() const { return derived()._impl(); }

  Derived& derived() { return static_cast<Derived&>(*this); }

  Derived const& derived() const { return static_cast<Derived const&>(*this); }

  virtual ~SeqBase() { };
};

}
}
#endif //SEQ_SEQBASE_H
