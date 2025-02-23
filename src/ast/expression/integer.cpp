#include "ast/expression/integer.hpp"

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "codegen/codegen.hpp"

llvm::Value *IntegerExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return llvm::ConstantInt::get(type->to_llvm(context->llvm_context), stoi(value));
}

void IntegerExpression::validate(const shared_ptr<ProgramContext> &context) {}
void IntegerExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  return resolve_types(context, nullptr);
}
void IntegerExpression::resolve_types(
    const shared_ptr<ProgramContext> &context,
    const shared_ptr<IntType> &destination_type) {
  set_type(destination_type ? destination_type : make_shared<IntType>(32, false));
}

string IntegerExpression::to_string() const { return value; }

string IntegerExpression::to_string_tree() const {
  return "IntegerExpression(type: " + type->to_string_tree() + ", value: '" +
         value + "')";
}
