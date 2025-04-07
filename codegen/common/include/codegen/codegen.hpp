#include <memory>

#include "codegen/codegen_backend.hpp"
#include "compiler/compiler_backend.hpp"
#include "program/program.hpp"

using namespace std;

class Codegen {
  shared_ptr<Program> program;

  CompilerBackend::Value backend;

 public:
  explicit Codegen(const shared_ptr<Program> &program,
                   const CompilerBackend::Value &backend)
      : program(program), backend(backend) {};

  void init() const;

  shared_ptr<CodegenBackend> get() const;
};
