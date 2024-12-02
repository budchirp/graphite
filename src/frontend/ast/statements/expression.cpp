#include <memory>
#include <sstream>

#include "frontend/ast/statements/expression.hpp"

ExpressionStatement::ExpressionStatement(unique_ptr<Expression> expression) {
  this->expression = std::move(expression);
}

std::string ExpressionStatement::to_string() const {
  std::ostringstream oss;
  oss << expression->to_string();
  return oss.str();
}

std::string ExpressionStatement::to_string_tree() const {
  std::ostringstream oss;
  oss << "ExpressionStatement(expression: " << expression->to_string_tree()
      << ")";
  return oss.str();
}
