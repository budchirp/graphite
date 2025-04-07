#include "ast/statement/function.hpp"

#include <memory>
#include <sstream>

#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/linkage_type.hpp"
#include "semantic/symbols/variable.hpp"
#include "semantic/type_helper.hpp"
#include "semantic/visibilty.hpp"

void FunctionStatement::validate(const shared_ptr<ProgramContext> &context) {
  context->get_env()->set_current_scope(scope->get_name());

  proto->validate(context);
  body->validate(context);

  auto return_type = proto->return_type->get_type();
  if (!TypeHelper::compare(return_type, make_shared<VoidType>()) &&
      !TypeHelper::compare(return_type, body->get_type())) {
    Logger::error(
        "Function `" + proto->name->value + "` \nexpected `" +
            return_type->to_string() + "` as return type but received `" +
            body->get_type()->to_string() + "`",
        LogTypes::Error::TYPE_MISMATCH, proto->return_type->get_position());
  }

  context->get_env()->set_current_scope(scope->get_parent()->get_name());
}

void FunctionStatement::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  proto->resolve_types(context);

  context->get_env()->set_current_scope(scope->get_name());

  vector<pair<string, shared_ptr<Type>>> parameters;
  for (const auto &[parameter_name_expression, parameter_type_expression] :
       proto->parameters) {
    auto name = parameter_name_expression->value;
    auto type = parameter_type_expression->get_type();
    scope->add_variable(
        name, make_shared<VariableSymbol>(
                  name, SymbolLinkageType::Value::Internal,
                  SymbolVisibility::Value::PRIVATE, type, false, false, true));

    parameters.emplace_back(name, type);
  }

  body->resolve_types(context);

  context->get_env()->set_current_scope(scope->get_parent()->get_name());

  auto name = proto->name->value;
  context->get_env()->add_function(
      name, make_shared<FunctionSymbol>(
                name, SymbolLinkageType::Value::Internal, visibilty,
                make_shared<FunctionType>(parameters,
                                          proto->return_type->get_type())));
}

string FunctionStatement::to_string() const {
  ostringstream oss;
  oss << "fn " << proto->to_string() << " " << body->to_string();
  return oss.str();
}

string FunctionStatement::to_string_tree() const {
  ostringstream oss;
  oss << "FunctionStatement(proto: " << proto->to_string_tree()
      << ", body: " << body->to_string_tree() << ")";
  return oss.str();
}
