#include "codegen_llvm/expression/struct.hpp"

#include <llvm/IR/Constants.h>

#include <cstddef>
#include <memory>

#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

llvm::Value* StructExpressionCodegen::codegen() const {
  auto struct_type = expression->get_struct_type();
  auto llvm_struct_type =
      LLVMCodegenUtils::type_to_llvm_type(context, struct_type);

  auto struct_ptr = context->builder->CreateAlloca(llvm_struct_type, nullptr);

  size_t idx = 0;
  for (const auto& [field_name, field_type] : struct_type->fields) {
    auto field = expression->fields.at(field_name);

    auto llvm_value = LLVMCodegen::codegen(context, field.second);
    if (!llvm_value) {
      Logger::error("Failed to generate low level code for struct value for `" +
                        field.first->value + "`",
                    LogTypes::Error::INTERNAL, field.second->get_position());
      return nullptr;
    }

    llvm_value = LLVMCodegenUtils::cast_type(
        context, llvm_value,
        LLVMCodegenUtils::type_to_llvm_type(context, field_type));
    if (!llvm_value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    field.second->get_position());
      return nullptr;
    }

    auto index_ptr =
        context->builder->CreateStructGEP(llvm_struct_type, struct_ptr, idx++);
    context->builder->CreateStore(llvm_value, index_ptr);
  }

  return struct_ptr;
}
