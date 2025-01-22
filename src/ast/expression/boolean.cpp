#include "ast/expression/boolean.hpp"

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "codegen/codegen.hpp"

using namespace llvm;

Value *BooleanExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return context->builder->getInt1(value);
}

void BooleanExpression::analyze(const shared_ptr<ProgramContext> &context) {}

string BooleanExpression::to_string() const { return ::to_string(value); }

string BooleanExpression::to_string_tree() const {
  return "BooleanExpression(type: " + type->to_string_tree() + ", value: '" +
         ::to_string(value) + "')";
}
