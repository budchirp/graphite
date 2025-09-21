#include "codegen_llvm/statement/include.hpp"
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Verifier.h>
#include "codegen_llvm/utils.hpp"

llvm::Value *IncludeStatementCodegen::codegen() const {
  auto env = context->get_env();
  auto scope = env->get_current_scope();

  for (const auto &[name, variable] : statement->program->get_context()
                                          ->get_env()
                                          ->get_scope("global")
                                          ->get_variables()) {
    if (name == "null") continue;

    if (variable->visibility == SymbolVisibility::Value::PUBLIC) {
      auto value = new llvm::GlobalVariable(
          *context->module, LLVMCodegenUtils::type_to_llvm_type(context, variable->type),
          false, llvm::GlobalValue::ExternalLinkage, nullptr, name);

      auto new_variable = scope->get_variable(name);
      new_variable->is_global = true;

      context->add_variable(name, value);

      scope->add_variable(name, new_variable);
    }
  }

  for (const auto &[name, function] :
       statement->program->get_context()->get_env()->get_functions()) {
    if (function->visibility == SymbolVisibility::Value::PUBLIC) {
      auto new_function = env->get_function(name);

      auto function = llvm::Function::Create(
          static_cast<llvm::FunctionType *>(
              LLVMCodegenUtils::type_to_llvm_type(context, new_function->type)),
          llvm::Function::ExternalLinkage, name, *context->module);

      size_t idx = 0;
      for (auto &argument : function->args())
        argument.setName(new_function->type->parameters[idx++].first);

      llvm::verifyFunction(*function);

      env->add_function(name, new_function);
    }
  }

  return llvm::Constant::getNullValue(
      llvm::Type::getVoidTy(*context->llvm_context));
}
