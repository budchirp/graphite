#include "ast/expression/var_ref.hpp"

#include "logger/log_types.hpp"
#include "logger/logger.hpp"

void VarRefExpression::validate(const shared_ptr<ProgramContext> &context) {}

void VarRefExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  auto variable = context->get_env()->get_current_scope()->get_variable(name);
  if (!variable || !variable->type) {
    Logger::error("Access to undefined variable", LogTypes::Error::UNDEFINED,
                  &position);
    return;
  }

  set_type(variable->type);
}

string VarRefExpression::to_string() const { return name; }

string VarRefExpression::to_string_tree() const {
  return "VarRefExpression(type: " + (type ? type->to_string_tree() : "") +
         ", name: '" + name + "')";
}
