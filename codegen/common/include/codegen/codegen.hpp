#pragma once

#include <memory>

#include "codegen/codegen_backend.hpp"
#include "program/program.hpp"

using namespace std;

class Codegen {
  shared_ptr<Program> program;

  shared_ptr<CodegenBackend> backend;

 public:
  explicit Codegen(const shared_ptr<Program> &program,
                   const shared_ptr<CodegenBackend> &backend)
      : program(program), backend(backend) {}

  shared_ptr<CodegenBackend> get() const { return backend; }

  void codegen() const;
};
