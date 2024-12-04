#include <string>

#include "frontend/ast/expressions/string.hpp"

StringExpression::StringExpression(string value) { this->value = value; }

string StringExpression::get_value() { return value; }

string StringExpression::to_string() const { return value; }

string StringExpression::to_string_tree() const {
  return "StringExpression(value: '" + value + "')";
}
