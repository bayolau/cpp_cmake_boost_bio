//
// Created by Bayo Lau on 8/11/15.
//

#include "dispatch.h"
#include "util/ProgramOptions.h"
#include "modules/sandbox/main.h"
#include "modules/bamtest/main.h"
#include "modules/vcftest/main.h"

namespace {
struct ModuleOptions : public bayolau::ProgramOptions {
  ModuleOptions(bayolau::CommandLine cl) {
    this->Add()("module,m", bayolau::bpo::value<std::string>(&module_)->required(), "module");
    this->AddPositional("module", 1);
    this->Parse(cl, 1, true);
  }

  const std::string& module() const {
    return module_;
  }

private:
  std::string module_;
};
}

const char* ModuleUsage() {
  return "usage: cmd module [...]";
}

namespace bayolau {

int Dispatch(int argc, const char* const argv[]) {
  bayolau::CommandLine cl(argc, argv);
  std::cout << cl << std::endl;
  ModuleOptions mo(cl);
  if (not mo.valid() or mo.help()) {
    std::cout << ModuleUsage() << std::endl;
    return 1;
  }
  std::cout << "module is " << mo.module() << std::endl;
  if (mo.module() == "sandbox") {
    return bayolau::sandbox::main(cl);
  }
  else if (mo.module() == "bamtest") {
    return bayolau::bamtest::main(cl);
  }
  else if (mo.module() == "vcftest") {
    return bayolau::vcftest::main(cl);
  }
  else {
    std::cerr << "unknown module " << ModuleUsage() << std::endl;
  }
  return 1;
}

}
