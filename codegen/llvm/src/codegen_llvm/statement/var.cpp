#include "codegen_llvm/statement/var.hpp"

#include <llvm/IR/Constants.h>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

llvm::Value *VarStatementCodegen::codegen() const {
  auto name = statement->name->value;

  auto scope = context->get_env()->get_current_scope();
  auto variable = scope->get_variable(name);

  auto llvm_type =
      LLVMCodegenUtils::type_to_llvm_type(context, statement->get_type());

  llvm::Value *value;
  if (variable->is_initialized) {
    value = LLVMCodegen::codegen(context, statement->expression);
    if (!value) {
      Logger::error("Failed to generate initializer for variable " + name,
                    LogTypes::Error::UNKNOWN,
                    statement->expression->get_position());
      return nullptr;
    }
  } else {
    value = nullptr;
  }

  if (value) {
    value = LLVMCodegenUtils::cast_type(context, value, llvm_type);
    if (!value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    statement->expression->get_position());
      return nullptr;
    }

    if (variable->is_global) {
      llvm::Constant *initializer = nullptr;
      if (auto *const_value = dyn_cast<llvm::Constant>(value)) {
        initializer = const_value;
      } else {
        initializer = llvm::UndefValue::get(llvm_type);
        Logger::warn(
            "Global variable '" + name +
                "' requires a constant initializer; using undefined value",
            LogTypes::Warn::SUGGESTION, statement->expression->get_position());
      }

      value = new llvm::GlobalVariable(
          *context->module, llvm_type, !variable->is_mutable,
          llvm::GlobalValue::ExternalLinkage, initializer);
    } else {
      auto ptr = context->builder->CreateAlloca(llvm_type, nullptr);
      context->builder->CreateStore(value, ptr);

      value = ptr;
    }
  } else {
    value = context->builder->CreateAlloca(llvm_type, nullptr);
  }

  context->add_variable(name, value);

  return llvm::UndefValue::get(llvm::Type::getVoidTy(*context->llvm_context));
}
