#include "ast/expression/var_ref.hpp"

#include "ast/expression/unary.hpp"
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

shared_ptr<VarRefExpression> VarRefExpression::is_var_ref(
    const shared_ptr<Expression> &expression) {
  if (auto var_ref_expression =
          dynamic_pointer_cast<VarRefExpression>(expression)) {
    return var_ref_expression;
  }

  if (auto unary_expression =
          dynamic_pointer_cast<UnaryExpression>(expression)) {
    return unary_expression->var_ref;
  }

  return nullptr;
}

string VarRefExpression::to_string() const { return name; }

string VarRefExpression::to_string_tree() const {
  return "VarRefExpression(type: " + (type ? type->to_string_tree() : "") +
         ", name: '" + name + "')";
}
