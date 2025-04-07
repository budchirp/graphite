#pragma once

#include "program/program.hpp"

class CodegenBackend {
 public:
  virtual ~CodegenBackend() = default;

  virtual void codegen(const shared_ptr<Program> &program) const = 0;
  virtual void optimize() const = 0;
};
