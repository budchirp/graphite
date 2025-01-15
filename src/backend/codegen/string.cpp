#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/string.hpp"

using namespace llvm;

Value *StringExpression::codegen() {
  return builder->CreateGlobalStringPtr(StringRef(value), "str", 0,
                                        module.get());
}
