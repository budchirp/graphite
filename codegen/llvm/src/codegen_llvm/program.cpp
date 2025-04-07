#include "codegen_llvm/program.hpp"

#include "codegen_llvm/codegen.hpp"

void ProgramCodegen::codegen() const {
  for (const auto &statement : program->statements) {
    LLVMCodegen::codegen(context, statement);
  }
}
