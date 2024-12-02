#include <memory>
#include <sstream>

#include "frontend/ast/statements/var.hpp"

VarStatement::VarStatement(unique_ptr<IdentifierExpression> name, unique_ptr<Expression> expression) {
  this->name = std::move(name);
  this->expression = std::move(expression);
}

std::string VarStatement::to_string() const {
  std::ostringstream oss;
  oss << "var " + name->to_string() + " = " + expression->to_string();
  return oss.str();
}

std::string VarStatement::to_string_tree() const {
  std::ostringstream oss;
  oss << "VarStatement(name: " + name->to_string_tree() + ", expression: "
      << expression->to_string_tree() << ")";
  return oss.str();
}
