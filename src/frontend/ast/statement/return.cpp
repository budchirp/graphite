#include <memory>

#include "frontend/ast/statement/return.hpp"

ReturnStatement::ReturnStatement(unique_ptr<Expression> expression) {
  this->expression = std::move(expression);
}

string ReturnStatement::to_string() const {
  return "return " + expression->to_string();
}

string ReturnStatement::to_string_tree() const {
  return "ReturnStatement(expression: " + expression->to_string_tree() + ")";
}
