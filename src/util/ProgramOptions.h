//
// Created by Bayo Lau on 8/10/15.
//


#ifndef UTIL_PROGRAM_OPTIONS_H
#define UTIL_PROGRAM_OPTIONS_H

#include <boost/program_options.hpp>

namespace bayolau {

  namespace bpo = boost::program_options;

  class ProgramOptions {
    bpo::options_description desc_;
    bpo::variables_map vm_;
    bool valid_;
  public:
    virtual ~ProgramOptions() { }

    template<typename T>
    T get(const std::string& flag) const {
      return vm_[flag].as<T>();
    }

    bool has(const std::string& flag) const {
      return vm_.count(flag);
    }

    friend std::ostream& operator<<(std::ostream& os, const ProgramOptions& other) {
      os << other.desc_;
      return os;
    }

    bool valid() const {
      return valid_;
    }

  protected:
    auto Add() -> decltype(this->desc_.add_options()) {
      return desc_.add_options();
    }

    void Parse(int argc, char* argv[]) {
      valid_ = false;
      vm_.clear();
      bpo::store(bpo::parse_command_line(argc, argv, desc_), vm_);
      try {
        bpo::notify(vm_);
        valid_ = true;
      }
      catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
      }
    }

    ProgramOptions() : desc_("Allowed options"), vm_(), valid_(false) {
      desc_.add_options()("help", "produce help message");
    }
  };
}


#endif //UTIL_PROGRAM_OPTIONS_H
