#include "codegen_llvm/expression/call.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

llvm::Value *CallExpressionCodegen::codegen() const {
  auto function = context->get_env()->get_function(expression->name->value);

  auto llvm_function = context->module->getFunction(function->name);
  if (!llvm_function) {
    Logger::error("Undefined function `" + function->name + "` called",
                  LogTypes::Error::UNDEFINED, expression->get_position());
    return nullptr;
  }

  vector<llvm::Value *> llvm_arguments;
  llvm_arguments.reserve(expression->arguments.size());

  size_t idx = 0;
  for (const auto &argument : expression->arguments) {
    auto llvm_argument = LLVMCodegen::codegen(context, argument);
    if (!llvm_argument) {
      Logger::error("Failed to generate low level code for argument `" +
                        argument->to_string() + "` in function `" +
                        function->name + "`",
                    LogTypes::Error::INTERNAL, argument->get_position());
      return nullptr;
    }

    llvm_argument = LLVMCodegenUtils::cast_type(
        context, llvm_argument,
        LLVMCodegenUtils::type_to_llvm_type(
            context, function->type->parameters[idx++].second));
    if (!llvm_argument) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    argument->get_position());
      return nullptr;
    }

    llvm_arguments.push_back(llvm_argument);
  }

  if (llvm_function->getReturnType()->isVoidTy()) {
    context->builder->CreateCall(llvm_function, llvm_arguments);
    return llvm::UndefValue::get(llvm::Type::getVoidTy(*context->llvm_context));
  } else {
    return context->builder->CreateCall(llvm_function, llvm_arguments);
  }
}
