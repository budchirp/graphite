#include "ast/statement/var.hpp"

#include <memory>
#include <sstream>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/symbols/variable.hpp"
#include "semantic/type_helper.hpp"

void VarStatement::validate(const shared_ptr<ProgramContext> &context) {
  if (variable_type) variable_type->validate(context);
  if (expression) {
    expression->validate(context);

    if (variable_type) {
      if (!TypeHelper::compare(variable_type->get_type(),
                               expression->get_type(), true)) {
        Logger::error(
            "Type mismatch\nExpected `" +
                variable_type->get_type()->to_string() + "` Received `" +
                expression->get_type()->to_string() + "`",
            LogTypes::Error::TYPE_MISMATCH, variable_type->get_position());
        return;
      }
    }
  }
}

void VarStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  if (variable_type) variable_type->resolve_types(context);
  if (expression) expression->resolve_types(context);

  if (variable_type) {
    type = variable_type->get_type();
  } else {
    type = expression->get_type();
  }

  auto scope = context->get_env()->get_current_scope();
  auto is_global = scope->get_name() == "global";

  scope->add_variable(
      name->value,
      make_shared<VariableSymbol>(
          name->value, SymbolLinkageType::Value::Internal,
          visibility, type, is_global, is_mutable, is_initialized));
}

string VarStatement::to_string() const {
  ostringstream oss;
  oss << "var " << name->to_string() << ": " << variable_type->to_string()
      << expression
      ? (" = " + expression->to_string())
      : "";
  return oss.str();
}

string VarStatement::to_string_tree() const {
  ostringstream oss;
  oss << "VarStatement(name: " + name->to_string_tree() + ", expression: "
      << (expression ? expression->to_string_tree() : "empty") << ", type: "
      << (variable_type ? variable_type->to_string_tree() : "empty") << ")";
  return oss.str();
}
