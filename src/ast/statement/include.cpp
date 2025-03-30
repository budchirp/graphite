#include "ast/statement/include.hpp"

#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include "codegen/codegen.hpp"
#include "compiler/compiler.hpp"
#include "semantic/visibilty.hpp"

using namespace llvm;

Value *IncludeStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto env = context->get_env();
  auto scope = env->get_current_scope();

  for (const auto &[name, variable] : program->get_context()
                                          ->get_env()
                                          ->get_scope("global")
                                          ->get_variables()) {
    if (name == "null") continue;

    if (variable->visibility == SymbolVisibility::Value::PUBLIC) {
      auto value = new GlobalVariable(
          *context->module, variable->type->to_llvm(context->llvm_context),
          false, GlobalValue::ExternalLinkage, nullptr, name);

      auto new_variable = scope->get_variable(name);
      new_variable->add_llvm_value(value);
      new_variable->is_global = true;

      scope->add_variable(name, new_variable);
    }
  }

  for (const auto &[name, function] :
       program->get_context()->get_env()->get_functions()) {
    if (function->visibility == SymbolVisibility::Value::PUBLIC) {
      auto new_function = env->get_function(name);

      auto function = Function::Create(
          static_cast<llvm::FunctionType *>(
              new_function->type->to_llvm(context->llvm_context)),
          Function::ExternalLinkage, name, *context->module);

      size_t idx = 0;
      for (auto &argument : function->args())
        argument.setName(new_function->type->parameters[idx++].first);

      verifyFunction(*function);

      new_function->add_llvm_value(function);

      env->add_function(name, new_function);
    }
  }

  return nullptr;
}

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

  env->add_include(module->get_value());

  program = Compiler::parse_program(context->get_path().parent_path(),
                                    module->get_value() + ".gph");

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
