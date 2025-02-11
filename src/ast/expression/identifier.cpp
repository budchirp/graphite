#include "ast/expression/identifier.hpp"

#include <alloca.h>
#include <llvm/IR/Value.h>

#include "codegen/codegen.hpp"

using namespace llvm;

Value *IdentifierExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return nullptr;
}

void IdentifierExpression::validate(const shared_ptr<ProgramContext> &context) {
}

void IdentifierExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {}

string IdentifierExpression::to_string() const { return value; }

string IdentifierExpression::to_string_tree() const {
  return "IdentifierExpression(value: '" + value + "')";
}
