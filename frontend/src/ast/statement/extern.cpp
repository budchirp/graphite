#include "ast/statement/extern.hpp"

#include <memory>

#include "semantic/linkage_type.hpp"

void ExternStatement::validate(const shared_ptr<ProgramContext> &context) {
  proto->validate(context);
}

void ExternStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  proto->resolve_types(context);

  vector<pair<string, shared_ptr<Type>>> parameters_type;
  parameters_type.reserve(proto->parameters.size());
  for (const auto &[parameter_name, parameter_type] : proto->parameters) {
    parameters_type.emplace_back(parameter_name->value,
                                 parameter_type->get_type());
  }

  auto name = proto->name->value;
  context->get_env()->add_function(
      name, make_shared<FunctionSymbol>(
                name, SymbolLinkageType::Internal, visibility,
                make_shared<FunctionType>(parameters_type,
                                          proto->return_type->get_type())));
}

string ExternStatement::to_string() const {
  return "extern " + proto->to_string();
}

string ExternStatement::to_string_tree() const {
  return "ExternStatement(proto: " + proto->to_string_tree() + ")";
}
