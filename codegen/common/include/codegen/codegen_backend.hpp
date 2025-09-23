#pragma once

#include <memory>

#include "program/program.hpp"

using namespace std;

class CodegenBackend {
 public:
  virtual ~CodegenBackend() = default;

  virtual void codegen(const shared_ptr<Program> &program) const = 0;
  virtual void optimize() const = 0;
};
