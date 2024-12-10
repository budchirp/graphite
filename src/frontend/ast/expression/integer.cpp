#include <string>

#include "frontend/ast/expression/integer.hpp"

IntegerExpression::IntegerExpression(const i32 &value) { this->value = value; }

i32 IntegerExpression::get_value() { return value; }

string IntegerExpression::to_string() const { return ::to_string(value); }

string IntegerExpression::to_string_tree() const {
  return "IntegerExpression(value: '" + ::to_string(value) + "')";
}
