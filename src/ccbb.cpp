#include <iostream>
#include "BuildInfo.h"
#include "modules/dispatch.h"
#include "util/Logging.h"



int main(int argc, const char* const argv[]) {
  LOG(info) << "build version " << bayolau::BuildInfo::version();
  LOG(info) << "git status " << bayolau::BuildInfo::git_status();
  return bayolau::modules::Dispatch(argc, argv);
}
