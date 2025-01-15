#include <memory>

#include "frontend/ast/statement/return.hpp"

ReturnStatement::ReturnStatement(const Position &position, unique_ptr<Expression> expression) 
    : position(position) {
  this->expression = std::move(expression);
}

string ReturnStatement::to_string() const {
  return "return " + expression->to_string();
}

string ReturnStatement::to_string_tree() const {
  return "ReturnStatement(expression: " + expression->to_string_tree() + ")";
}
