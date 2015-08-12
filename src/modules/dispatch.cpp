//
// Created by Bayo Lau on 8/11/15.
//

#include "dispatch.h"
#include "util/ProgramOptions.h"
#include "modules/sandbox/main.h"

namespace {
struct ModuleOptions : public bayolau::ProgramOptions {
  ModuleOptions(int argc, const char* const argv[]) {
    this->AddPositional("module", 1);
    this->Add()("module,m", bayolau::bpo::value<std::string>(&module_)->required(), "module");
    this->Parse(std::min(2, argc), argv, true);
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
  ModuleOptions mo(argc, argv);
  if (not mo.valid() or mo.help()) {
    std::cout << ModuleUsage() << std::endl;
    return 1;
  }
  std::cout << "module is " << mo.module() << std::endl;
  if (mo.module() == "sandbox") {
    return bayolau::sandbox::main(argc, argv);

  }
  return 0;
}

}
