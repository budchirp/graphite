#include <string>

#include "frontend/ast/expression/string.hpp"

StringExpression::StringExpression(const Position &position,
                                   const string &value): position(position) {
  this->value = value;
}

string StringExpression::get_value() { return value; }

string StringExpression::to_string() const { return value; }

string StringExpression::to_string_tree() const {
  return "StringExpression(value: '" + value + "')";
}
