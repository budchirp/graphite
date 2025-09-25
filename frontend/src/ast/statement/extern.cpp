#include "ast/statement/extern.hpp"

#include <memory>

#include "ast/statement/proto.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/linkage_type.hpp"

void ExternStatement::validate(const shared_ptr<ProgramContext> &context) {
  proto->validate(context);
}

void ExternStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  proto->resolve_types(context);

  auto env = context->get_env();
  auto scope = env->get_current_scope();

  if (auto function_proto_statement =
          dynamic_pointer_cast<FunctionProtoStatement>(proto)) {
    vector<pair<string, shared_ptr<Type>>> parameters_type;
    parameters_type.reserve(function_proto_statement->parameters.size());
    for (const auto &[parameter_name, parameter_type] :
         function_proto_statement->parameters) {
      parameters_type.emplace_back(parameter_name->value,
                                   parameter_type->get_type());
    }

    auto name = function_proto_statement->name->value;
    env->add_function(
        name, make_shared<FunctionSymbol>(
                  name, SymbolLinkageType::Internal, visibility,
                  make_shared<FunctionType>(
                      parameters_type,
                      function_proto_statement->return_type->get_type())));
  } else if (auto var_proto_statement =
                 dynamic_pointer_cast<VarProtoStatement>(proto)) {
    auto name = var_proto_statement->name->value;
    scope->add_variable(
        name, make_shared<VariableSymbol>(
                  name, SymbolLinkageType::Value::External, visibility,
                  var_proto_statement->get_type(), true, false, true));
  } else {
    Logger::error("Unknown proto statement", LogTypes::Error::INTERNAL,
                  proto->get_position());
  }
}

string ExternStatement::to_string() const {
  return "extern " + proto->to_string();
}

string ExternStatement::to_string_tree() const {
  return "ExternStatement(proto: " + proto->to_string_tree() + ")";
}
