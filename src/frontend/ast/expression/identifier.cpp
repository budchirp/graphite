#include "frontend/ast/expression/identifier.hpp"

IdentifierExpression::IdentifierExpression(const string &value) {
  this->value = value;
}

string IdentifierExpression::get_value() { return value; }

string IdentifierExpression::to_string() const { return value; }

string IdentifierExpression::to_string_tree() const {
  return "IdentifierExpression(value: '" + value + "')";
}
