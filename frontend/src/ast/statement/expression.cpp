#include "ast/statement/expression.hpp"

#include <memory>

void ExpressionStatement::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);
}

void ExpressionStatement::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  expression->resolve_types(context);

  type = expression->get_type();
}

string ExpressionStatement::to_string() const {
  return expression->to_string();
}

string ExpressionStatement::to_string_tree() const {
  return "ExpressionStatement(expression: " + expression->to_string_tree() +
         ")";
}
