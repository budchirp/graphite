#include <string>

#include "frontend/ast/expressions/boolean.hpp"

BooleanExpression::BooleanExpression(bool value) {
  this->value = std::move(value);
}

bool BooleanExpression::get_value() { return value; }

string BooleanExpression::to_string() const { return ::to_string(value); }

string BooleanExpression::to_string_tree() const {
  return "BooleanExpression(value: '" + ::to_string(value) + "')";
}
