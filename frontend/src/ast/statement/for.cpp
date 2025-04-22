#include "ast/statement/for.hpp"

#include <memory>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"
#include "types/boolean.hpp"

void ForStatement::validate(const shared_ptr<ProgramContext> &context) {
  init->validate(context);

  condition->validate(context);
  if (!TypeHelper::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  increment->validate(context);

  body->validate(context);
}

void ForStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  init->resolve_types(context);
  condition->resolve_types(context);
  increment->resolve_types(context);

  body->resolve_types(context);
}

string ForStatement::to_string() const {
  return "for (" + init->to_string() + "; " + condition->to_string() + "; " +
         increment->to_string() + ") " + body->to_string();
}

string ForStatement::to_string_tree() const {
  return "ForStatement(init: " + init->to_string_tree() +
         ", condition: " + condition->to_string_tree() +
         ", increment: " + increment->to_string_tree() +
         ", body: " + body->to_string_tree() + ")";
}
