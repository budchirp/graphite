#include "codegen_llvm/expression/index.hpp"

#include <memory>
#include <string>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/options.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

llvm::Value* IndexExpressionCodegen::codegen() const {
  return codegen(nullptr);
}

llvm::Value* IndexExpressionCodegen::codegen(
    const shared_ptr<CodegenOptions>& options) const {
  auto index_value = LLVMCodegen::codegen(context, expression->index);
  if (!index_value) {
    Logger::error("Failed to generate low level code for index",
                  LogTypes::Error::INTERNAL, expression->index->get_position());
    return nullptr;
  }

  auto array_ptr =
      LLVMCodegen::codegen(context, expression->expression,
                           make_shared<CodegenOptions>(false, true));

  auto index_ptr = context->builder->CreateGEP(
      LLVMCodegenUtils::type_to_llvm_type(context,
                                          expression->get_array_type()),
      array_ptr, {context->builder->getInt32(0), index_value});

  if (options && !options->load_variable) {
    return index_ptr;
  } else {
    return context->builder->CreateLoad(
        LLVMCodegenUtils::type_to_llvm_type(context, expression->get_type()),
        index_ptr);
  }
}
