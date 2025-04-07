#pragma once

#include <memory>

#include "program/program.hpp"

using namespace std;

class Validator {
 private:
  shared_ptr<Program> program;

 public:
  explicit Validator(const shared_ptr<Program> &program) : program(program) {}

  void validate() const { program->validate(); };
};
