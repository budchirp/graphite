#include "ast/expression/string.hpp"

#include <llvm/IR/Value.h>

#include <string>

#include "codegen/codegen.hpp"

using namespace llvm;

Value *StringExpression::codegen() const {
  return context->builder->CreateGlobalStringPtr(StringRef(value), "str", 0,
                                                 context->module.get());
}

string StringExpression::to_string() const { return value; }

string StringExpression::to_string_tree() const {
  return "StringExpression(value: '" + value + "')";
}
