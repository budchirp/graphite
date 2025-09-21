#include "codegen_llvm/expression/call.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

llvm::Value *CallExpressionCodegen::codegen() const {
  auto name = expression->name->value;

  auto function = context->module->getFunction(name);
  if (!function) {
    Logger::error("Undefined function `" + name + "` called",
                  LogTypes::Error::UNDEFINED, expression->get_position());
    return nullptr;
  }

  vector<llvm::Value *> llvm_arguments;

  size_t idx = 0;
  for (const auto &argument : expression->arguments) {
    auto llvm_argument = LLVMCodegen::codegen(context, argument);
    if (!llvm_argument) {
      Logger::error("Failed to generate low level code for argument `" +
                        argument->to_string() + "` in function `" + name + "`",
                    LogTypes::Error::INTERNAL, argument->get_position());
      return nullptr;
    }

    auto function = context->get_env()->get_function(name);
    auto parameter_type = function->type->parameters[idx++].second;

    llvm_argument = LLVMCodegenUtils::cast_type(
        context, llvm_argument,
        LLVMCodegenUtils::type_to_llvm_type(context, parameter_type));
    if (!llvm_argument) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    argument->get_position());
      return nullptr;
    }

    llvm_arguments.push_back(llvm_argument);
  }

  if (function->getReturnType()->isVoidTy()) {
    context->builder->CreateCall(function, llvm_arguments);
    return llvm::Constant::getNullValue(
        llvm::Type::getVoidTy(*context->llvm_context));
  } else {
    return context->builder->CreateCall(function, llvm_arguments);
  }
}
