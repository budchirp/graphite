#include <memory>
#include <sstream>

#include "frontend/ast/statement/var.hpp"
#include "frontend/lexer/position.hpp"

VarStatement::VarStatement(const Position &position,
                           unique_ptr<IdentifierExpression> name,
                           unique_ptr<IdentifierExpression> type,
                           unique_ptr<Expression> expression)
    : position(position) {
  this->name = std::move(name);
  this->type = std::move(type);
  this->expression = std::move(expression);
}

string VarStatement::to_string() const {
  ostringstream oss;
  oss << "var " << name->to_string() << ": " << type->to_string() << " = "
      << expression->to_string();
  return oss.str();
}

string VarStatement::to_string_tree() const {
  ostringstream oss;
  oss << "VarStatement(name: " + name->to_string_tree() + ", expression: "
      << expression->to_string_tree() << ", type: " << type->to_string_tree()
      << ")";
  return oss.str();
}
