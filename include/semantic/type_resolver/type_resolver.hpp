#pragma once

#include <memory>

#include "ast/program.hpp"

using namespace std;

class TypeResolver {
 private:
  shared_ptr<Program> program;

 public:
  explicit TypeResolver(const shared_ptr<Program> &program)
      : program(program) {};

  void resolve() { program->resolve_types(); };
};
