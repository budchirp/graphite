#include "ast/expression/assembly.hpp"

#include <cstddef>
#include <memory>
#include <string>

#include "types/int.hpp"

void AssemblyExpression::validate(const shared_ptr<ProgramContext> &context) {
  assembly->validate(context);

  for (const auto &[constraint, variable] : output) {
    constraint->validate(context);
    variable->validate(context);
  }

  for (const auto &[constraint, variable] : input) {
    constraint->validate(context);
    variable->validate(context);
  }

  for (const auto &reg : clobbered) {
    reg->validate(context);
  }
}

void AssemblyExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  assembly->resolve_types(context);

  auto scope = context->get_env()->get_current_scope();

  size_t i = 0;
  vector<pair<string, shared_ptr<Type>>> types;
  for (const auto &[constraint_expression, variable_expression] : output) {
    constraint_expression->resolve_types(context);
    variable_expression->resolve_types(context);

    auto variable = scope->get_variable(variable_expression->value);
    types.emplace_back(constraint_expression->value + ::to_string(i++),
                       variable->type);
  }

  for (const auto &[constraint_expression, variable_expression] : input) {
    constraint_expression->resolve_types(context);
    variable_expression->resolve_types(context);

    auto variable = scope->get_variable(variable_expression->value);
    types.emplace_back(constraint_expression->value + ::to_string(i++),
                       variable->type);
  }

  for (const auto &reg : clobbered) {
    reg->resolve_types(context);
  }

  set_function_type(make_shared<FunctionType>(types, make_shared<IntType>(64, false)));
  set_type(function_type->return_type);
}

string AssemblyExpression::to_string() const { return "asm"; }

string AssemblyExpression::to_string_tree() const {
  return "AssemblyExpression()";
}
