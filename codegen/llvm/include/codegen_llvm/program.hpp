#pragma once

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Value.h>

#include <memory>

#include "codegen_llvm/context.hpp"
#include "program/program.hpp"

using namespace std;

class ProgramCodegen {
 private:
  shared_ptr<LLVMCodegenContext> context;

  shared_ptr<Program> program;

 public:
  explicit ProgramCodegen(const shared_ptr<LLVMCodegenContext>& context,
                          const shared_ptr<Program>& program)
      : context(context), program(program) {};

  void codegen() const;
};
