#include "ast/statement/return.hpp"

#include <memory>

#include "logger/logger.hpp"

void ReturnStatement::validate(const shared_ptr<ProgramContext> &context) {
  expression->validate(context);
}

void ReturnStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  expression->resolve_types(context);

  type = expression->get_type();
}

string ReturnStatement::to_string() const {
  return "return " + expression->to_string();
}

string ReturnStatement::to_string_tree() const {
  return "ReturnStatement(type: " + (type ? type->to_string_tree() : "") +
         ", expression: " + expression->to_string_tree() + ")";
}
