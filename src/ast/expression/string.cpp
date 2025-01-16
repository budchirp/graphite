#include <string>
#include <llvm/IR/Value.h>

#include "codegen/codegen.hpp"
#include "ast/expression/string.hpp"

using namespace llvm;

Value *StringExpression::codegen() const {
  return builder->CreateGlobalStringPtr(StringRef(value), "str", 0,
                                        module.get());
}

string StringExpression::to_string() const { return value; }

string StringExpression::to_string_tree() const {
  return "StringExpression(value: '" + value + "')";
}
