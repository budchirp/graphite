#include <memory>

#include "frontend/ast/statement/expression.hpp"

ExpressionStatement::ExpressionStatement(const Position &position, unique_ptr<Expression> expression) 
    : position(position) {
  this->expression = std::move(expression);
}

string ExpressionStatement::to_string() const {
  return expression->to_string();
}

string ExpressionStatement::to_string_tree() const {
  return "ExpressionStatement(expression: " + expression->to_string_tree() +
         ")";
}
