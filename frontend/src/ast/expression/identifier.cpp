#include "ast/expression/identifier.hpp"

#include <memory>

#include "ast/expression/unary.hpp"

void IdentifierExpression::validate(const shared_ptr<ProgramContext> &context) {
}

void IdentifierExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {}

shared_ptr<IdentifierExpression> IdentifierExpression::is_identifier(
    const shared_ptr<Expression> &expression) {
  if (auto identifier_expression =
          dynamic_pointer_cast<IdentifierExpression>(expression)) {
    return identifier_expression;
  }

  if (auto unary_expression =
          dynamic_pointer_cast<UnaryExpression>(expression)) {
    return unary_expression->identifier;
  }

  return nullptr;
};

string IdentifierExpression::to_string() const { return value; }

string IdentifierExpression::to_string_tree() const {
  return "IdentifierExpression(value: '" + value + "')";
}
