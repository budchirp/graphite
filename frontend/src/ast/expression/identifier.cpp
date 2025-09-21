#include "ast/expression/identifier.hpp"

#include <memory>

#include "ast/expression/unary.hpp"

void IdentifierExpression::validate(const shared_ptr<ProgramContext> &context) {
}

void IdentifierExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {}

string IdentifierExpression::to_string() const { return value; }

string IdentifierExpression::to_string_tree() const {
  return "IdentifierExpression(value: '" + value + "')";
}
