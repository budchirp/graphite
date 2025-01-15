#include <llvm/IR/Value.h>

#include "frontend/ast/program.hpp"

using namespace llvm;

Value *Program::codegen() {
  for (const auto &statement : statements) {
    statement->codegen();
  }

  return nullptr;
}
