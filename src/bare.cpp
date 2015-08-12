#include <iostream>
#include "BuildInfo.h"
#include "modules/dispatch.h"


int main(int argc, const char* const argv[]) {
  std::cout << "build version " << bayolau::BuildInfo::version() << std::endl;
  std::cout << "git status " << bayolau::BuildInfo::git_status() << std::endl;
  bayolau::Dispatch(argc, argv);
}
