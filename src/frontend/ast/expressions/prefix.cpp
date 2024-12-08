#include <string>

#include "frontend/ast/expressions/prefix.hpp"

PrefixExpression::PrefixExpression(Token prefix, unique_ptr<Expression> right) {
  this->prefix = prefix;

  this->right = std::move(right);
}

string PrefixExpression::to_string() const {
  return prefix.to_string() + right->to_string();
}

string PrefixExpression::to_string_tree() const {
  return "PrefixExpression(op: " + prefix.to_string_tree() +
         ", right: " + right->to_string_tree() + ")";
}
