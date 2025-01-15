#include <memory>

#include <llvm/IR/Value.h>

#include "ast/statement/expression.hpp"
#include "codegen/codegen.hpp"

using namespace llvm;

Value *ExpressionStatement::codegen() { return expression->codegen(); }

string ExpressionStatement::to_string() const {
  return expression->to_string();
}

string ExpressionStatement::to_string_tree() const {
  return "ExpressionStatement(expression: " + expression->to_string_tree() +
         ")";
}
