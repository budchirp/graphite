#include <memory>
#include <sstream>

#include "frontend/ast/statements/return.hpp"

ReturnStatement::ReturnStatement(unique_ptr<Expression> expression) {
  this->expression = std::move(expression);
}

std::string ReturnStatement::to_string() const {
  std::ostringstream oss;
  oss << "return ";
  oss << expression->to_string();
  return oss.str();
}

std::string ReturnStatement::to_string_tree() const {
  std::ostringstream oss;
  oss << "ReturnStatement(expression: " << expression->to_string_tree()
      << ")";
  return oss.str();
}
