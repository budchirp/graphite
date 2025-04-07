#include "ast/statement/include.hpp"

#include "compiler/compiler.hpp"

void IncludeStatement::validate(const shared_ptr<ProgramContext> &context) {
  module->validate(context);
}

void IncludeStatement::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  module->resolve_types(context);

  auto env = context->get_env();
  auto scope = env->get_current_scope();

  if (scope->get_name() != "global") {
    return;
  }

  env->add_include(module->value);

  program = Compiler::parse_program(context->get_path().parent_path(),
                                    module->value + ".gph");

  for (const auto &[name, variable] : program->get_context()
                                          ->get_env()
                                          ->get_scope("global")
                                          ->get_variables()) {
    if (variable->visibility == SymbolVisibility::Value::PUBLIC) {
      auto new_variable = variable;
      new_variable->linkage = SymbolLinkageType::Value::External;

      scope->add_variable(name, new_variable);
    }
  }

  for (const auto &[name, function] :
       program->get_context()->get_env()->get_functions()) {
    if (function->visibility == SymbolVisibility::Value::PUBLIC) {
      auto new_function = function;
      new_function->linkage = SymbolLinkageType::Value::External;

      env->add_function(name, new_function);
    }
  }
}

string IncludeStatement::to_string() const { return ""; }

string IncludeStatement::to_string_tree() const { return ""; }
