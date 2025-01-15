#include <llvm/IR/Value.h>
#include <string>

#include "ast/expression/integer.hpp"
#include "codegen/codegen.hpp"

using namespace llvm;

Value *IntegerExpression::codegen() {
  return ConstantInt::get(llvm::Type::getInt32Ty(*context), value, true);
}

string IntegerExpression::to_string() const { return ::to_string(value); }

string IntegerExpression::to_string_tree() const {
  return "IntegerExpression(value: '" + ::to_string(value) + "')";
}
