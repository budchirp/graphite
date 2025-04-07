#include "codegen_llvm/expression/array.hpp"

#include <memory>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

llvm::Value* ArrayExpressionCodegen::codegen() const {
  auto llvm_array_type = LLVMCodegenUtils::type_to_llvm_type(context, expression->get_type());
  auto array =
      context->builder->CreateAlloca(llvm_array_type, nullptr, "array");

  size_t idx = 0;
  for (const auto& value : expression->values) {
    auto llvm_value = LLVMCodegen::codegen(context, value);
    if (!llvm_value) {
      Logger::error(
          "Failed to generate low level code for array value at index `" +
              ::to_string(idx) + "`",
          LogTypes::Error::INTERNAL, value->get_position());
      return nullptr;
    }

    llvm_value = LLVMCodegenUtils::cast_type(
        context, llvm_value,
        LLVMCodegenUtils::type_to_llvm_type(context, expression->get_array_type()->child_type));
    if (!llvm_value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    value->get_position());
      return nullptr;
    }

    auto index = context->builder->getInt32(idx++);
    auto index_ptr = context->builder->CreateGEP(
        llvm_array_type, array, {context->builder->getInt32(0), index},
        "array_index");
    context->builder->CreateStore(llvm_value, index_ptr);
  }

  return array;
}
