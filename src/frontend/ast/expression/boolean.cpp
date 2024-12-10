#include <string>

#include "frontend/ast/expression/boolean.hpp"

BooleanExpression::BooleanExpression(const bool value) { this->value = value; }

bool BooleanExpression::get_value() { return value; }

string BooleanExpression::to_string() const { return ::to_string(value); }

string BooleanExpression::to_string_tree() const {
  return "BooleanExpression(value: '" + ::to_string(value) + "')";
}
