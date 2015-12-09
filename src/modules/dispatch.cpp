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
// Created by Bayo Lau on 8/11/15.
//

#include "dispatch.h"
#include "util/ProgramOptions.h"
#include "util/Logging.h"
#include "modules/modules.h"

namespace bayolau {
namespace modules {

namespace {
struct ModuleOptions : public ProgramOptions {
  ModuleOptions(CommandLine cl) {
    this->Add()("module,m", bpo::value<std::string>(&module_)->default_value(""), Modules::Desc().c_str());
    this->AddPositional("module", 1);
    this->Parse(cl, 1, true);
  }

  const std::string& module() const {
    return module_;
  }

  bool valid() const {
    return ProgramOptions::valid() and Modules::Has(module_);
  }

private:
  std::string module_;
};
}

int Dispatch(int argc, const char* const argv[]) {
  CommandLine cl(argc, argv);
  LOG(info) << cl;
  ModuleOptions mo(cl);

  std::ostream& os = mo.help() ? std::cout : std::cerr;
  if (not mo.valid() or mo.help()) {
    os << mo << std::endl;
    return 1;
  }

  return Modules::Run(mo.module(), cl);
}
}
}
