#include <llvm/IR/Value.h>
#include <string>

#include "ast/expression/boolean.hpp"
#include "codegen/codegen.hpp"

using namespace llvm;

Value *BooleanExpression::codegen() const { return builder->getInt1(value); }

string BooleanExpression::to_string() const { return ::to_string(value); }

string BooleanExpression::to_string_tree() const {
  return "BooleanExpression(value: '" + ::to_string(value) + "')";
}
