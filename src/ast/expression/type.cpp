#include <string>

#include "ast/expression/type.hpp"

llvm::Value *TypeExpression::codegen() const { return nullptr; }

string TypeExpression::to_string() const { return type->to_string(); }

string TypeExpression::to_string_tree() const {
  return "TypeExpression(type: " + type->to_string_tree() + ")";
}
