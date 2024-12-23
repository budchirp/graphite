#include <string>
#include <utility>

#include "frontend/ast/expression/binary.hpp"

BinaryExpression::BinaryExpression(const Token &op, unique_ptr<Expression> left,
                                   unique_ptr<Expression> right) {

  this->op = op;

  this->left = std::move(left);
  this->right = std::move(right);
}

string BinaryExpression::to_string() const {
  return "(" + left->to_string() + " " + op.to_string() + " " +
         right->to_string() + ")";
}

string BinaryExpression::to_string_tree() const {
  return "BinaryExpression(op: " + op.to_string() +
         ", left: " + left->to_string_tree() +
         ", right: " + right->to_string_tree() + ")";
}
