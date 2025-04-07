#include "ast/statement/while.hpp"

#include <memory>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

void WhileStatement::validate(const shared_ptr<ProgramContext> &context) {
  condition->validate(context);
  if (!TypeHelper::compare(condition->get_type(), make_shared<BooleanType>())) {
    Logger::error("Only booleans are allowed on condition",
                  LogTypes::Error::TYPE_MISMATCH, condition->get_position());
    return;
  }

  body->validate(context);
}

void WhileStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  condition->resolve_types(context);

  body->resolve_types(context);
}

string WhileStatement::to_string() const {
  return "while (" + condition->to_string() + ") " + body->to_string();
}

string WhileStatement::to_string_tree() const {
  return "WhileStatement(condition: " + condition->to_string_tree() +
         ", body: " + body->to_string_tree() + ")";
}
