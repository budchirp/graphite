#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/integer.hpp"

using namespace llvm;

Value *IntegerExpression::codegen() {
  return ConstantInt::get(Type::getInt32Ty(*context), value, true);
}
