#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/boolean.hpp"

using namespace llvm;

Value *BooleanExpression::codegen() { return builder->getInt1(value); }
