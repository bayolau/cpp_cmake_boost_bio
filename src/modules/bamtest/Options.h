//
// Created by Bayo Lau on 8/12/15.
//

#ifndef MODULES_BAMTEST_OPTIONS_H
#define MODULES_BAMTEST_OPTIONS_H

namespace bayolau {
namespace bamtest {
struct Options : public bayolau::ProgramOptions {
  Options(const bayolau::CommandLine& cl) {
    this->Add()("module,m", bayolau::bpo::value<std::string>(&module_)->required(), "module");
    this->Add()("input,i", bayolau::bpo::value<std::string>(&input_)->required(), "input file");
    this->Add()("fasta,f", bayolau::bpo::value<std::string>(&fasta_)->required(), "fasta file");
    this->AddPositional("module", 1);
    this->AddPositional("input", 1);
    this->Parse(cl);
  }

  const std::string& input() const {
    return input_;
  }

  const std::string& fasta() const {
    return fasta_;
  }

private:
  std::string input_;
  std::string fasta_;
  std::string module_;
};
}
}

#endif //MODULES_BAMTEST_OPTIONS_H
