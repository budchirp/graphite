#include "codegen_llvm/statement/return.hpp"

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

llvm::Value *ReturnStatementCodegen::codegen() const {
  if (auto value = LLVMCodegen::codegen(context, statement->expression)) {
    auto function = context->builder->GetInsertBlock()->getParent();
    auto return_type = function->getReturnType();

    value = LLVMCodegenUtils::cast_type(context, value, return_type);
    if (!value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    statement->get_position());
      return nullptr;
    }

    context->builder->CreateRet(value);
    return value;
  }

  Logger::error("Failed to generate low level code for return value",
                LogTypes::Error::UNKNOWN,
                statement->expression->get_position());
  return nullptr;
}
