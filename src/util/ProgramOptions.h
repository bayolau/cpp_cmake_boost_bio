//
// Created by Bayo Lau on 8/10/15.
//


#ifndef UTIL_PROGRAM_OPTIONS_H
#define UTIL_PROGRAM_OPTIONS_H

#include <iostream>
#include <boost/program_options.hpp>

namespace bayolau {

namespace bpo = boost::program_options;

class ProgramOptions {
  bpo::positional_options_description p_desc_;
  bpo::options_description desc_;
  bpo::variables_map vm_;
  bool valid_;
public:
  virtual ~ProgramOptions() { }


  friend std::ostream& operator<<(std::ostream& os, const ProgramOptions& other) {
    os << other.desc_;
    return os;
  }

  bool valid() const {
    return valid_;
  }

  bool help() const {
    return this->has("help");
  }

protected:
  template<typename T>
  T get(const std::string& flag) const {
    return vm_[flag].as<T>();
  }

  bool has(const std::string& flag) const {
    return vm_.count(flag);
  }

  auto Add() -> decltype(desc_.add_options()) {
    return desc_.add_options();
  }

  void AddPositional(const char* name, int max_count) {
    p_desc_.add(name, max_count);
  }

  void Parse(int argc, const char* const argv[], bool allow_unregistered = false) {
    valid_ = false;
    vm_.clear();
    auto parser = bpo::command_line_parser(argc, argv).options(desc_).positional(p_desc_);
    bpo::store(allow_unregistered ? parser.allow_unregistered().run() : parser.run(), vm_);
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
