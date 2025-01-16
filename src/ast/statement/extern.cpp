#include <memory>
#include <llvm/IR/Value.h>

#include "ast/statement/extern.hpp"
#include "ast/statement/proto.hpp"
#include "codegen/codegen.hpp"

using namespace llvm;

Value *ExternStatement::codegen() const { return proto->codegen_function(); }

string ExternStatement::to_string() const {
  return "extern " + proto->to_string();
}

string ExternStatement::to_string_tree() const {
  return "ExternStatement(proto: " + proto->to_string_tree() + ")";
}
