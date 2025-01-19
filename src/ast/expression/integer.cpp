#include "ast/expression/integer.hpp"

#include <llvm/IR/Value.h>

#include <string>

#include "codegen/codegen.hpp"

using namespace llvm;

Value *IntegerExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return ConstantInt::get(type->to_llvm(context->llvm_context), value, true);
}

void IntegerExpression::analyze(const shared_ptr<ProgramContext> &context) {}

string IntegerExpression::to_string() const { return ::to_string(value); }

string IntegerExpression::to_string_tree() const {
  return "IntegerExpression(type: " + type->to_string_tree() + ", value: '" +
         ::to_string(value) + "')";
}
