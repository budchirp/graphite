#include <string>

#include "frontend/ast/expression/integer.hpp"

IntegerExpression::IntegerExpression(const Position &position,
                                     const int &value) : position(position) {
  this->value = value;
}

int IntegerExpression::get_value() { return value; }

string IntegerExpression::to_string() const { return ::to_string(value); }

string IntegerExpression::to_string_tree() const {
  return "IntegerExpression(value: '" + ::to_string(value) + "')";
}
