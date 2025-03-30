#pragma once

#include <memory>

#include "program/program.hpp"

using namespace std;

class TypeResolver {
 private:
  shared_ptr<Program> program;

 public:
  explicit TypeResolver(const shared_ptr<Program> &program)
      : program(program) {};

  void resolve() const { program->resolve_types(); };
};
