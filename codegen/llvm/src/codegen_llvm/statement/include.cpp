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
    if (variable->visibility == SymbolVisibility::Value::PUBLIC) {
      auto value = new llvm::GlobalVariable(
          *context->module,
          LLVMCodegenUtils::type_to_llvm_type(context, variable->type), false,
          llvm::GlobalValue::ExternalLinkage, nullptr, name);

      auto new_variable = scope->get_variable(name);
      new_variable->is_global = true;

      context->add_variable(name, value);

      scope->add_variable(name, new_variable);
    }
  }

  for (const auto &[name, function] :
       statement->program->get_context()->get_env()->get_functions()) {
    if (function->visibility == SymbolVisibility::Value::PUBLIC) {
      auto llvm_function_type = static_cast<llvm::FunctionType *>(
          LLVMCodegenUtils::type_to_llvm_type(context, function->type));

      auto llvm_function = llvm::Function::Create(
          llvm_function_type, llvm::Function::ExternalLinkage, name,
          *context->module);

      size_t idx = 0;
      for (auto &argument : llvm_function->args())
        argument.setName(function->type->parameters[idx++].first);

      llvm::verifyFunction(*llvm_function);

      env->add_function(name, function);
    }
  }

  return llvm::UndefValue::get(llvm::Type::getVoidTy(*context->llvm_context));
}
